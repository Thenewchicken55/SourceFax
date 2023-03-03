#include "XMLParser.hpp"
#include "xml_parser.hpp"
#include <bitset>
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

constexpr auto WHITESPACE = " \n\t\r"sv;
constexpr auto NAMEEND = "> /\":=\n\t\r"sv;

// constructor
XMLParser::XMLParser(std::string_view content)
    : content(content)
    {}

// get doneReading
bool XMLParser::isDoneReading() {
    return doneReading;
}

// set doneReading
// @param[in] change doneReading to newValue
void XMLParser::setDoneReading(bool newBool) {
    doneReading = newBool;
}

// get qName
std::string_view XMLParser::getQName() {
    return qName;
}

// set qName
void XMLParser::setQName(std::string_view newQName) {
    qName = newQName;
}

// get prefix
std::string_view XMLParser::getPrefix() {
    return prefix;
}

// get localName
std::string_view XMLParser::getLocalName() {
    return localName;
}

// get inEscape
bool XMLParser::inEscape() {
    return (localName == "escape"sv);
}

// get characters
std::string_view XMLParser::getCharacters() {
    return characters;
}

// parse file from the start
int XMLParser::parseBegin() {
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

// parse XML declaration
void XMLParser::parseXMLDeclaration() {
    content.remove_prefix("<?xml"sv.size());
    content.remove_prefix(content.find_first_not_of(WHITESPACE));

    // parse required version
    parseVersion();
    
    // parse optional encoding and standalone attributes
    parseEncodingAndStandalone();

    TRACE("XML DECLARATION", "version", version, "encoding", (encoding ? *encoding : ""), "standalone", (standalone ? *standalone : ""));
    assert(content.compare(0, "?>"sv.size(), "?>"sv) == 0);
    content.remove_prefix("?>"sv.size());
    content.remove_prefix(content.find_first_not_of(WHITESPACE));
}

// parse required version
void XMLParser::parseVersion() {
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
void XMLParser::parseEncodingAndStandalone() {
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
void XMLParser::parseDOCTYPE() {
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
int XMLParser::refillPreserve() {
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

// parse character entity references
void XMLParser::parseCharacterEntityReference() {
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
void XMLParser::parseCharacterNotEntityReference() {
    assert(content[0] != '<' && content[0] != '&');
    const auto characterEndPosition = content.find_first_of("<&");
    characters = (content.substr(0, characterEndPosition));\
    TRACE("CHARACTERS", "characters", characters);
    content.remove_prefix(characters.size());
}

// parse XML comment
int XMLParser::parseComment() {
    int bytesRead = 0;
    assert(content.compare(0, "<!--"sv.size(), "<!--"sv) == 0);
    content.remove_prefix("<!--"sv.size());
    auto tagEndPosition = content.find("-->"sv);
    if (tagEndPosition == content.npos) {
        // refill content preserving unprocessed
        bytesRead = refillPreserve();     
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
int XMLParser::parseCDATA() {
    int bytesRead = 0;
    content.remove_prefix("<![CDATA["sv.size());
    auto tagEndPosition = content.find("]]>"sv);
    if (tagEndPosition == content.npos) {
        // refill content preserving unprocessed
        bytesRead = refillPreserve();
        tagEndPosition = content.find("]]>"sv);
        if (tagEndPosition == content.npos) {
            std::cerr << "parser error : Unterminated CDATA\n";
            exit(1);
        }
    }
    characters = (content.substr(0, tagEndPosition));
    content.remove_prefix(tagEndPosition);
    content.remove_prefix("]]>"sv.size());
    TRACE("CDATA", "characters", characters);
    return bytesRead;
}

// parse processing instruction
void XMLParser::parseProcessing() {
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
void XMLParser::parseEndTag() {
    xml_parser::parseEndTag(content);
}

// parse start tag
void XMLParser::parseStartTag() {
    auto nameEndPosition = xml_parser::parseStartTag(content, qName, prefix, localName);
    content.remove_prefix(nameEndPosition);
}

// parse XML namespace
void XMLParser::parseNamespace() {
    xml_parser::parseNamespace(content);
}

// parse attribute
std::string_view XMLParser::parseAttribute() {
    return xml_parser::parseAttribute(content);
}

// Accessor::predicate to test if the tag is a XML declaration
bool XMLParser::isXML() {
    return xml_parser::isXML(content);
}

// Accessor::predicate to test if the tag is DOCTYPE
bool XMLParser::isDOCTYPE() {
    return xml_parser::isDOCTYPE(content);
}

// Accessor::predicate to test if the tag is CDATA
bool XMLParser::isCDATA() {
    return xml_parser::isCDATA(content);
}

// Accessor::predicate to test if the tag is a comment tag
bool XMLParser::isComment() {
    return xml_parser::isComment(content);
}

// Accessor::predicate to test if the tag is an XML namespace
bool XMLParser::isNamespace() {
    return xml_parser::isNamespace(content);
}

// Accessor::predicate to check if content has a specific character at a specific index
bool XMLParser::isCharacter(int index, char character) {
    return xml_parser::isCharacter(content, index, character);
}

// wrapper for remove_prefix()
void XMLParser::removePrefix(int index) {
    content.remove_prefix(index);
}

// wrapper for find_first_not_of()
std::size_t XMLParser::findFirstNotOf(std::string_view string) {
    return content.find_first_not_of(string);
}

// wrapper for size()
int XMLParser::sizeOfContent() {
    return content.size();
}

// wrapper for .npos
size_t XMLParser::npos() {
    return content.npos;
}

// wrapper for compare()
int XMLParser::compareContent(int position, int count, const std::string& string) {
    return content.compare(position, count, string);
}

// uses xmlNameMask bitset to compare with the first char of content 
bool XMLParser::isMatchNameMask() {
    const std::bitset<128> xmlNameMask("00000111111111111111111111111110100001111111111111111111111111100000001111111111011000000000000000000000000000000000000000000000");
    return xmlNameMask[content[0]];
}
