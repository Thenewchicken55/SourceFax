/*
    xml_parser.cpp

    Implementation file for low-level XML parse functions
*/

#include "xml_parser.hpp"
#include "refillContent.hpp"
#include <string_view>
#include <cassert>
#include <iostream>
#include <optional>
#include <utility>
#include <algorithm>
#include <utility>
#include <iomanip>

// trace parsing
#ifdef TRACE
#undef TRACE
#define HEADER(m) std::clog << "\033[1m" << std::setw(10) << std::left << m << "\u001b[0m" << '\t'
#define TRACE0() ""
#define TRACE1(l1, n1)                         "\033[1m" << l1 << "\u001b[0m" << "|" << "\u001b[31;1m" << n1 << "\u001b[0m" << "| "
#define TRACE2(l1, n1, l2, n2)                 TRACE1(l1,n1)             << TRACE1(l2,n2)
#define TRACE3(l1, n1, l2, n2, l3, n3)         TRACE2(l1,n1,l2,n2)       << TRACE1(l3,n3)
#define TRACE4(l1, n1, l2, n2, l3, n3, l4, n4) TRACE3(l1,n1,l2,n2,l3,n3) << TRACE1(l4,n4)
#define GET_TRACE(_2,_3,_4,_5,_6,_7,_8,_9,NAME,...) NAME
#define TRACE(m,...) HEADER(m) << GET_TRACE(__VA_ARGS__, TRACE4, _UNUSED, TRACE3, _UNUSED, TRACE2, _UNUSED, TRACE1, TRACE0, TRACE0)(__VA_ARGS__) << '\n';
#else
#define TRACE(...)
#endif

// provides literal string operator""sv
using namespace std::literals::string_view_literals;

namespace xml_parser {

    constexpr auto WHITESPACE = " \n\t\r"sv;
    std::optional<std::string_view> encoding;
    std::optional<std::string_view> standalone;
    [[maybe_unused]] const std::string_view version;
    
    // parse XML declaration
    void parseXMLDeclaration(std::string_view& content) {
        assert(content.compare(0, "<?xml "sv.size(), "<?xml "sv) == 0);
        content.remove_prefix("<?xml"sv.size());
        content.remove_prefix(content.find_first_not_of(WHITESPACE));

        // parse required version
        parseVersion(content);
        
        // parse optional encoding and standalone attributes
        parseEncodingAndStandalone(content);

        TRACE("XML DECLARATION", "version", version, "encoding", (encoding ? *encoding : ""), "standalone", (standalone ? *standalone : ""));
        assert(content.compare(0, "?>"sv.size(), "?>"sv) == 0);
        content.remove_prefix("?>"sv.size());
        content.remove_prefix(content.find_first_not_of(WHITESPACE));

    }

    // parse required version
    void parseVersion(std::string_view& content) {
        const auto nameEndPosition = content.find_first_of("= ");
        const auto attr(content.substr(0, nameEndPosition));
        content.remove_prefix(nameEndPosition);
        content.remove_prefix(content.find_first_not_of(WHITESPACE));
        content.remove_prefix("="sv.size());
        content.remove_prefix(content.find_first_not_of(WHITESPACE));
        const auto delimiter = content[0];
        if (delimiter != '"' && delimiter != '\'') {
            std::cerr << "parser error: Invalid start delimiter for version in XML declaration\n";
            exit(1);
        }
        content.remove_prefix("\""sv.size());
        const auto valueEndPosition = content.find(delimiter);
        if (valueEndPosition == content.npos) {
            std::cerr << "parser error: Invalid end delimiter for version in XML declaration\n";
            exit(1);
        }
        if (attr != "version"sv) {
            std::cerr << "parser error: Missing required first attribute version in XML declaration\n";
            exit(1);
        }
        [[maybe_unused]] const auto version(content.substr(0, valueEndPosition));
        content.remove_prefix(valueEndPosition);
        content.remove_prefix("\""sv.size());
        content.remove_prefix(content.find_first_not_of(WHITESPACE));
    }


    // parse optional encoding and standalone attributes
    void parseEncodingAndStandalone(std::string_view& content) {
        if (content[0] != '?') {
            const auto nameEndPosition = content.find_first_of("= ");
            if (nameEndPosition == content.npos) {
                std::cerr << "parser error: Incomplete attribute in XML declaration\n";
                exit(1);
            }
            const auto attr2(content.substr(0, nameEndPosition));
            content.remove_prefix(nameEndPosition);
            content.remove_prefix(content.find_first_not_of(WHITESPACE));
            assert(content.compare(0, "="sv.size(), "="sv) == 0);
            content.remove_prefix("="sv.size());
            content.remove_prefix(content.find_first_not_of(WHITESPACE));
            const auto delimiter2 = content[0];
            if (delimiter2 != '"' && delimiter2 != '\'') {
                std::cerr << "parser error: Invalid end delimiter for attribute " << attr2 << " in XML declaration\n";
                exit(1);
            }
            content.remove_prefix("\""sv.size());
            const auto valueEndPosition = content.find(delimiter2);
            if (valueEndPosition == content.npos) {
                std::cerr << "parser error: Incomplete attribute " << attr2 << " in XML declaration\n";
                exit(1);
            }
            if (attr2 == "encoding"sv) {
                encoding = content.substr(0, valueEndPosition);
            } else if (attr2 == "standalone"sv) {
                standalone = content.substr(0, valueEndPosition);
            } else {
                std::cerr << "parser error: Invalid attribute " << attr2 << " in XML declaration\n";
                exit(1);
            }
            content.remove_prefix(valueEndPosition + 1);
            content.remove_prefix(content.find_first_not_of(WHITESPACE));
        }
        if (content[0] != '?') {
            const auto nameEndPosition = content.find_first_of("= ");
            if (nameEndPosition == content.npos) {
                std::cerr << "parser error: Incomplete attribute in XML declaration\n";
                exit(1);
            }
            const auto attr2(content.substr(0, nameEndPosition));
            content.remove_prefix(nameEndPosition);
            content.remove_prefix(content.find_first_not_of(WHITESPACE));
            content.remove_prefix("="sv.size());
            content.remove_prefix(content.find_first_not_of(WHITESPACE));
            const auto delimiter2 = content[0];
            if (delimiter2 != '"' && delimiter2 != '\'') {
                std::cerr << "parser error: Invalid end delimiter for attribute " << attr2 << " in XML declaration\n";
                exit(1);
            }
            content.remove_prefix("\""sv.size());
            const auto valueEndPosition = content.find(delimiter2);
            if (valueEndPosition == content.npos) {
                std::cerr << "parser error: Incomplete attribute " << attr2 << " in XML declaration\n";
                exit(1);
            }
            if (!standalone && attr2 == "standalone"sv) {
                standalone = content.substr(0, valueEndPosition);
            } else {
                std::cerr << "parser error: Invalid attribute " << attr2 << " in XML declaration\n";
                exit(1);
            }
            // assert(content[valueEndPosition + 1] == '"');
            content.remove_prefix(valueEndPosition + 1);
            content.remove_prefix(content.find_first_not_of(WHITESPACE));
        }
    }

    //parse DOCTYPE
    void parseDOCTYPE(std::string_view& content) {
        assert(content.compare(0, "<!DOCTYPE "sv.size(), "<!DOCTYPE "sv) == 0);
        content.remove_prefix("<!DOCTYPE"sv.size());
        int depthAngleBrackets = 1;
        bool inSingleQuote = false;
        bool inDoubleQuote = false;
        bool inComment = false;
        std::size_t p = 0;
        while ((p = content.find_first_of("<>'\"-"sv, p)) != content.npos) {
            if (content.compare(p, "<!--"sv.size(), "<!--"sv) == 0) {
                inComment = true;
                p += "<!--"sv.size();
                continue;
            } else if (content.compare(p, "-->"sv.size(), "-->"sv) == 0) {
                inComment = false;
                p += "-->"sv.size();
                continue;
            }
            if (inComment) {
                ++p;
                continue;
            }
            if (content[p] == '<' && !inSingleQuote && !inDoubleQuote) {
                ++depthAngleBrackets;
            } else if (content[p] == '>' && !inSingleQuote && !inDoubleQuote) {
                --depthAngleBrackets;
            } else if (content[p] == '\'') {
                inSingleQuote = !inSingleQuote;
            } else if (content[p] == '"') {
                inDoubleQuote = !inDoubleQuote;
            }
            if (depthAngleBrackets == 0)
                break;
            ++p;
        }
        [[maybe_unused]] const auto contents(content.substr(0, p));
        TRACE("DOCTYPE", "contents", contents);
        content.remove_prefix(p);
        assert(content[0] == '>');
        content.remove_prefix(">"sv.size());
        content.remove_prefix(content.find_first_not_of(WHITESPACE));
    }

    // refill content preserving unprocessed
    int refillPreserve(std::string_view& content, bool& doneReading) {
        int bytesRead = refillContent(content);
        if (bytesRead < 0) {
            std::cerr << "parser error : File input error\n";
            exit(1);
        }
        if (bytesRead == 0) {
            doneReading = true;
        }
        
        return bytesRead;
    }

    std::string_view unescapedCharacter;
    std::string_view escapedCharacter;

    // parse character entity references
    void parseCharacterEntityReference(std::string_view& content) {

        if (content[1] == 'l' && content[2] == 't' && content[3] == ';') {
            unescapedCharacter = "<";
            escapedCharacter = "&lt;"sv;
        } else if (content[1] == 'g' && content[2] == 't' && content[3] == ';') {
            unescapedCharacter = ">";
            escapedCharacter = "&gt;"sv;
        } else if (content[1] == 'a' && content[2] == 'm' && content[3] == 'p' && content[4] == ';') {
            unescapedCharacter = "&";
            escapedCharacter = "&amp;"sv;
        } else {
            unescapedCharacter = "&";
            escapedCharacter = "&"sv;
        }
        assert(content.compare(0, escapedCharacter.size(), escapedCharacter) == 0);
        content.remove_prefix(escapedCharacter.size());
        [[maybe_unused]] const auto characters(unescapedCharacter);
        TRACE("CHARACTERS", "characters", characters);
    }

    // parse character non-entity references
    std::string_view parseCharacterNotEntityReference(std::string_view& content) {
        assert(content[0] != '<' && content[0] != '&');
        const auto characterEndPosition = content.find_first_of("<&");
        const auto characters(content.substr(0, characterEndPosition));\
        TRACE("CHARACTERS", "characters", characters);
        content.remove_prefix(characters.size());
        return characters;
    }

    // parse XML comment
    int parseComment(std::string_view& content, bool& doneReading) {
        int bytesRead = 0;
        assert(content.compare(0, "<!--"sv.size(), "<!--"sv) == 0);
        content.remove_prefix("<!--"sv.size());
        auto tagEndPosition = content.find("-->"sv);
        if (tagEndPosition == content.npos) {
            // refill content preserving unprocessed
            bytesRead = refillPreserve(content, doneReading);     
            tagEndPosition = content.find("-->"sv);
            if (tagEndPosition == content.npos) {
                std::cerr << "parser error : Unterminated XML comment\n";
                exit(1);
            }
        }
        [[maybe_unused]] const auto comment(content.substr(0, tagEndPosition));
        TRACE("COMMENT", "content", comment);
        content.remove_prefix(tagEndPosition);
        assert(content.compare(0, "-->"sv.size(), "-->"sv) == 0);
        content.remove_prefix("-->"sv.size());
        content.remove_prefix(content.find_first_not_of(WHITESPACE) == content.npos ? content.size() : content.find_first_not_of(WHITESPACE));

        return bytesRead;
    }

    // parse CDATA
    std::pair<int, std::string_view> parseCDATA(std::string_view& content, bool& doneReading) {
        int bytesRead = 0;
        content.remove_prefix("<![CDATA["sv.size());
        auto tagEndPosition = content.find("]]>"sv);
        if (tagEndPosition == content.npos) {
            // refill content preserving unprocessed
            bytesRead = refillPreserve(content, doneReading);
            tagEndPosition = content.find("]]>"sv);
            if (tagEndPosition == content.npos) {
                std::cerr << "parser error : Unterminated CDATA\n";
                exit(1);
            }
        }
        const auto characters(content.substr(0, tagEndPosition));
        content.remove_prefix(tagEndPosition);
        content.remove_prefix("]]>"sv.size());
        TRACE("CDATA", "characters", characters);
        return std::make_pair(bytesRead, characters);
    }

    constexpr auto NAMEEND = "> /\":=\n\t\r"sv;

    // parse processing instruction
    void parseProcessing(std::string_view& content) {
        assert(content.compare(0, "<?"sv.size(), "<?"sv) == 0);
        content.remove_prefix("<?"sv.size());
        const auto tagEndPosition = content.find("?>"sv);
        if (tagEndPosition == content.npos) {
            std::cerr << "parser error: Incomplete XML declaration\n";
            exit(1);
        }
        auto nameEndPosition = content.find_first_of(NAMEEND);
        if (nameEndPosition == content.npos) {
            std::cerr << "parser error : Unterminated processing instruction\n";
            exit(1);
        }
        [[maybe_unused]] const auto target(content.substr(0, nameEndPosition));
        [[maybe_unused]] const auto data(content.substr(nameEndPosition, tagEndPosition - nameEndPosition));
        TRACE("PI", "target", target, "data", data);
        content.remove_prefix(tagEndPosition);
        assert(content.compare(0, "?>"sv.size(), "?>"sv) == 0);
        content.remove_prefix("?>"sv.size()); 
    }

    // parse end tag
    void parseEndTag(std::string_view& content) {
        assert(content.compare(0, "</"sv.size(), "</"sv) == 0);
        content.remove_prefix("</"sv.size());
        if (content[0] == ':') {
            std::cerr << "parser error : Invalid end tag name\n";
            exit(1);
        }
        auto nameEndPosition = content.find_first_of(NAMEEND);
        if (nameEndPosition == content.size()) {
            std::cerr << "parser error : Unterminated end tag '" << content.substr(0, nameEndPosition) << "'\n";
            exit(1);
        }
        std::size_t colonPosition = 0;
        if (content[nameEndPosition] == ':') {
            colonPosition = nameEndPosition;
            nameEndPosition = content.find_first_of(NAMEEND, nameEndPosition + 1);
        }
        const auto qName(content.substr(0, nameEndPosition));
        if (qName.empty()) {
            std::cerr << "parser error: EndTag: invalid element name\n";
            exit(1);
        }
        [[maybe_unused]] const auto prefix(qName.substr(0, colonPosition));
        [[maybe_unused]] const auto localName(qName.substr(colonPosition ? colonPosition + 1 : 0));
        TRACE("END TAG", "qName", qName, "prefix", prefix, "localName", localName);
        content.remove_prefix(nameEndPosition);
        content.remove_prefix(content.find_first_not_of(WHITESPACE));
        assert(content.compare(0, ">"sv.size(), ">"sv) == 0);
        content.remove_prefix(">"sv.size());
    }

    // parse start tag
    std::size_t parseStartTag(std::string_view& content, std::string_view& qName, std::string_view& prefix, std::string_view& localName) {
        assert(content.compare(0, "<"sv.size(), "<"sv) == 0);
        content.remove_prefix("<"sv.size());
        if (content[0] == ':') {
            std::cerr << "parser error : Invalid start tag name\n";
            exit(1);
        }
        auto nameEndPosition = content.find_first_of(NAMEEND);
        if (nameEndPosition == content.size()) {
            std::cerr << "parser error : Unterminated start tag '" << content.substr(0, nameEndPosition) << "'\n";
            exit(1);
        }
        std::size_t colonPosition = 0;
        if (content[nameEndPosition] == ':') {
            colonPosition = nameEndPosition;
            nameEndPosition = content.find_first_of(NAMEEND, nameEndPosition + 1);
        }
        qName = content.substr(0, nameEndPosition);
        if (qName.empty()) {
            std::cerr << "parser error: StartTag: invalid element name\n";
            exit(1);
        }
        prefix = qName.substr(0, colonPosition);
        localName = qName.substr(colonPosition ? colonPosition + 1 : 0, nameEndPosition);
        TRACE("START TAG", "qName", qName, "prefix", prefix, "localName", localName);

        return nameEndPosition;
    }

    // parse XML namespace
    void parseNamespace(std::string_view& content) {
        assert(content.compare(0, "xmlns"sv.size(), "xmlns"sv) == 0);
        content.remove_prefix("xmlns"sv.size());
        auto nameEndPosition = content.find('=');
        if (nameEndPosition == content.npos) {
            std::cerr << "parser error : incomplete namespace\n";
            exit(1);
        }
        std::size_t prefixSize = 0;
        if (content[0] == ':') {
            content.remove_prefix(":"sv.size());
            --nameEndPosition;
            prefixSize = nameEndPosition;
        }
        [[maybe_unused]] const auto prefix(content.substr(0, prefixSize));
        content.remove_prefix(nameEndPosition);
        content.remove_prefix("="sv.size());
        content.remove_prefix(content.find_first_not_of(WHITESPACE));
        if (content.empty()) {
            std::cerr << "parser error : incomplete namespace\n";
            exit(1);
        }
        const auto delimiter = content[0];
        if (delimiter != '"' && delimiter != '\'') {
            std::cerr << "parser error : incomplete namespace\n";
            exit(1);
        }
        content.remove_prefix("\""sv.size());
        const auto valueEndPosition = content.find(delimiter);
        if (valueEndPosition == content.npos) {
            std::cerr << "parser error : incomplete namespace\n";
            exit(1);
        }
        [[maybe_unused]] const auto uri(content.substr(0, valueEndPosition));
        TRACE("NAMESPACE", "prefix", prefix, "uri", uri);
        content.remove_prefix(valueEndPosition);
        assert(content.compare(0, "\""sv.size(), "\""sv) == 0);
        content.remove_prefix("\""sv.size());
        content.remove_prefix(content.find_first_not_of(WHITESPACE));
    }

    // parse attribute
    std::size_t parseAttribute(std::string_view& content) {
        auto nameEndPosition = content.find_first_of(NAMEEND);
        if (nameEndPosition == content.size()) {
            std::cerr << "parser error : Empty attribute name" << '\n';
            exit(1);
        }
        std::size_t colonPosition = 0;
        if (content[nameEndPosition] == ':') {
            colonPosition = nameEndPosition;
            nameEndPosition = content.find_first_of(NAMEEND, nameEndPosition + 1);
        }
        const auto qName(content.substr(0, nameEndPosition));
        [[maybe_unused]] const auto prefix(qName.substr(0, colonPosition));
        [[maybe_unused]] const auto localName(qName.substr(colonPosition ? colonPosition + 1 : 0));
        content.remove_prefix(nameEndPosition);
        content.remove_prefix(content.find_first_not_of(WHITESPACE));
        if (content.empty()) {
            std::cerr << "parser error : attribute " << qName << " incomplete attribute\n";
            exit(1);
        }
        if (content[0] != '=') {
            std::cerr << "parser error : attribute " << qName << " missing =\n";
            exit(1);
        }
        content.remove_prefix("="sv.size());
        content.remove_prefix(content.find_first_not_of(WHITESPACE));
        const auto delimiter = content[0];
        if (delimiter != '"' && delimiter != '\'') {
            std::cerr << "parser error : attribute " << qName << " missing delimiter\n";
            exit(1);
        }
        content.remove_prefix("\""sv.size());
        const auto valueEndPosition = content.find(delimiter);
        if (valueEndPosition == content.npos) {
            std::cerr << "parser error : attribute " << qName << " missing delimiter\n";
            exit(1);
        }
        return valueEndPosition;
    }

    // Accessor::predicate to test if the tag is a XML declaration
    bool isXML(std::string_view content) {
        return content[0] == '<' && content[1] == '?' && content[2] == 'x' && content[3] == 'm' && content[4] == 'l' && content[5] == ' ';
    }

    // Accessor::predicate to test if the tag is DOCTYPE
    bool isDOCTYPE(std::string_view content) {
        return content[1] == '!' && content[0] == '<' && content[2] == 'D' && content[3] == 'O' && content[4] == 'C' && content[5] == 'T' && content[6] == 'Y' && content[7] == 'P' && content[8] == 'E' && content[9] == ' ';
    }

    // Accessor::predicate to test if the tag is CDATA
    bool isCDATA(std::string_view content) {
        return content[1] == '!' /* && content[0] == '<' */ && content[2] == '[' && content[3] == 'C' && content[4] == 'D' && content[5] == 'A' && content[6] == 'T' && content[7] == 'A' && content[8] == '[';
    }

    // Accessor::predicate to test if the tag is a comment tag
    bool isComment(std::string_view content) {
        return (!content.empty() && content[0] == '<' && content[1] == '!' && content[2] == '-' && content[3] == '-') || (content[1] == '!' /* && content[0] == '<' */ && content[2] == '-' && content[3] == '-');
    }

    // Accessor::predicate to test if the tag is an XML namespace
    bool isNamespace(std::string_view content) {
        return content[0] == 'x' && content[1] == 'm' && content[2] == 'l' && content[3] == 'n' && content[4] == 's' && (content[5] == ':' || content[5] == '=');
    }

    // parse file from the start
    int parseBegin(std::string_view& content) {
        TRACE("START DOCUMENT");
        const int bytesRead = refillContent(content);
        if (bytesRead < 0) {
            std::cerr << "parser error : File input error\n";
            return 1;
        }
        if (bytesRead == 0) {
            std::cerr << "parser error : Empty file\n";
            return 1;
        }

        return bytesRead;
    }

    // Accessor::predicate to check if content has a specific character at a specific index
    bool isCharacter(std::string_view& content, int index, char character) {
        return content[index] == character;
    }
}
