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

#define TRACE(...)

// provides literal string operator""sv
using namespace std::literals::string_view_literals;

constexpr auto WHITESPACE = " \n\t\r"sv;
std::optional<std::string_view> encoding;
std::optional<std::string_view> standalone;

// parse XML declaration
void parseXMLDeclaration(std::string_view& content)
{
    assert(content.compare(0, "<?xml "sv.size(), "<?xml "sv) == 0);
    content.remove_prefix("<?xml"sv.size());
    content.remove_prefix(content.find_first_not_of(WHITESPACE));

    // parse required version
    parseVersion(content);
    
    // parse optional encoding and standalone attributes
    parseOptional(content);

}

// parse required version
void parseVersion(std::string_view& content) {
    auto nameEndPosition = content.find_first_of("= ");
    const std::string_view attr(content.substr(0, nameEndPosition));
    content.remove_prefix(nameEndPosition);
    content.remove_prefix(content.find_first_not_of(WHITESPACE));
    content.remove_prefix("="sv.size());
    content.remove_prefix(content.find_first_not_of(WHITESPACE));
    const char delimiter = content[0];
    if (delimiter != '"' && delimiter != '\'') {
        std::cerr << "parser error: Invalid start delimiter for version in XML declaration\n";
        exit(1);
    }
    content.remove_prefix("\""sv.size());
    auto valueEndPosition = content.find(delimiter);
    if (valueEndPosition == content.npos) {
        std::cerr << "parser error: Invalid end delimiter for version in XML declaration\n";
        exit(1);
    }
    if (attr != "version"sv) {
        std::cerr << "parser error: Missing required first attribute version in XML declaration\n";
        exit(1);
    }
    [[maybe_unused]] const std::string_view version(content.substr(0, valueEndPosition));
    content.remove_prefix(valueEndPosition);
    content.remove_prefix("\""sv.size());
    content.remove_prefix(content.find_first_not_of(WHITESPACE));
}


// parse optional encoding and standalone attributes
void parseOptional(std::string_view& content) {
    
    if (content[0] != '?') {
        auto nameEndPosition = content.find_first_of("= ");
        if (nameEndPosition == content.npos) {
            std::cerr << "parser error: Incomplete attribute in XML declaration\n";
            exit(1);
        }
        const std::string_view attr2(content.substr(0, nameEndPosition));
        content.remove_prefix(nameEndPosition);
        content.remove_prefix(content.find_first_not_of(WHITESPACE));
        assert(content.compare(0, "="sv.size(), "="sv) == 0);
        content.remove_prefix("="sv.size());
        content.remove_prefix(content.find_first_not_of(WHITESPACE));
        char delimiter2 = content[0];
        if (delimiter2 != '"' && delimiter2 != '\'') {
            std::cerr << "parser error: Invalid end delimiter for attribute " << attr2 << " in XML declaration\n";
            exit(1);
        }
        content.remove_prefix("\""sv.size());
        auto valueEndPosition = content.find(delimiter2);
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
        auto nameEndPosition = content.find_first_of("= ");
        if (nameEndPosition == content.npos) {
            std::cerr << "parser error: Incomplete attribute in XML declaration\n";
            exit(1);
        }
        const std::string_view attr2(content.substr(0, nameEndPosition));
        content.remove_prefix(nameEndPosition);
        content.remove_prefix(content.find_first_not_of(WHITESPACE));
        content.remove_prefix("="sv.size());
        content.remove_prefix(content.find_first_not_of(WHITESPACE));
        const char delimiter2 = content[0];
        if (delimiter2 != '"' && delimiter2 != '\'') {
            std::cerr << "parser error: Invalid end delimiter for attribute " << attr2 << " in XML declaration\n";
            exit(1);
        }
        content.remove_prefix("\""sv.size());
        auto valueEndPosition = content.find(delimiter2);
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
    TRACE("XML DECLARATION", "version", version, "encoding", (encoding ? *encoding : ""), "standalone", (standalone ? *standalone : ""));
    assert(content.compare(0, "?>"sv.size(), "?>"sv) == 0);
    content.remove_prefix("?>"sv.size());
    content.remove_prefix(content.find_first_not_of(WHITESPACE));
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
    [[maybe_unused]] const std::string_view contents(content.substr(0, p));
    TRACE("DOCTYPE", "contents", contents);
    content.remove_prefix(p);
    assert(content[0] == '>');
    content.remove_prefix(">"sv.size());
    content.remove_prefix(content.find_first_not_of(WHITESPACE));
}

// refill content preserving unprocessed
void refillPreserve(std::string_view& content, bool& doneReading, long& totalBytes) {
    int bytesRead = refillContent(content);
    if (bytesRead < 0) {
        std::cerr << "parser error : File input error\n";
        exit(1);
    }
    if (bytesRead == 0) {
        doneReading = true;
    }
    totalBytes += bytesRead;
}

std::string_view unescapedCharacter;
std::string_view escapedCharacter;

// parse character entity references
void parseCharER(std::string_view& content) {

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
    [[maybe_unused]] const std::string_view characters(unescapedCharacter);
    TRACE("CHARACTERS", "characters", characters);
}

// parse character non-entity references
void parseCharNonER(std::string_view& content, int& loc, int& textSize) {
    assert(content[0] != '<' && content[0] != '&');
    auto characterEndPosition = content.find_first_of("<&");
    const std::string_view characters(content.substr(0, characterEndPosition));
    TRACE("CHARACTERS", "characters", characters);
    loc += static_cast<int>(std::count(characters.cbegin(), characters.cend(), '\n'));
    textSize += static_cast<int>(characters.size());
    content.remove_prefix(characters.size());
}

// parse XML comment
void parseComment(std::string_view& content, bool& doneReading, long& totalBytes)
{
    assert(content.compare(0, "<!--"sv.size(), "<!--"sv) == 0);
    content.remove_prefix("<!--"sv.size());
    auto tagEndPosition = content.find("-->"sv);
    if (tagEndPosition == content.npos) {
        // refill content preserving unprocessed
        
        tagEndPosition = content.find("-->"sv);
        if (tagEndPosition == content.npos) {
            std::cerr << "parser error : Unterminated XML comment\n";
            exit(1);
        }
    }
    [[maybe_unused]] const std::string_view comment(content.substr(0, tagEndPosition));
    TRACE("COMMENT", "content", comment);
    content.remove_prefix(tagEndPosition);
    content.remove_prefix("-->"sv.size());
}

// parse CDATA
void parseCDATA(std::string_view& content, bool& doneReading, long& totalBytes, int& textSize, int& loc)
{
    content.remove_prefix("<![CDATA["sv.size());
    auto tagEndPosition = content.find("]]>"sv);
    if (tagEndPosition == content.npos) {
        // refill content preserving unprocessed
        refillPreserve(content, doneReading, totalBytes);
        tagEndPosition = content.find("]]>"sv);
        if (tagEndPosition == content.npos) {
            std::cerr << "parser error : Unterminated CDATA\n";
            exit(1);
        }
    }
    const std::string_view characters(content.substr(0, tagEndPosition));
    TRACE("CDATA", "characters", characters);
    textSize += static_cast<int>(characters.size());
    loc += static_cast<int>(std::count(characters.cbegin(), characters.cend(), '\n'));
    content.remove_prefix(tagEndPosition);
    content.remove_prefix("]]>"sv.size());
}

constexpr auto NAMEEND = "> /\":=\n\t\r"sv;

// parse processing instruction
void parseProcessing(std::string_view& content)
{
    assert(content.compare(0, "<?"sv.size(), "<?"sv) == 0);
    content.remove_prefix("<?"sv.size());
    auto tagEndPosition = content.find("?>"sv);
    if (tagEndPosition == content.npos) {
        std::cerr << "parser error: Incomplete XML declaration\n";
        exit(1);
    }
    auto nameEndPosition = content.find_first_of(NAMEEND);
    if (nameEndPosition == content.npos) {
        std::cerr << "parser error : Unterminated processing instruction\n";
        exit(1);
    }
    [[maybe_unused]] const std::string_view target(content.substr(0, nameEndPosition));
    [[maybe_unused]] const std::string_view data(content.substr(nameEndPosition, tagEndPosition - nameEndPosition));
    TRACE("PI", "target", target, "data", data);
    content.remove_prefix(tagEndPosition);
    assert(content.compare(0, "?>"sv.size(), "?>"sv) == 0);
    content.remove_prefix("?>"sv.size()); 
}

// parse end tag
void parseEndTag(std::string_view& content)
{
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
    const std::string_view qName(content.substr(0, nameEndPosition));
    if (qName.empty()) {
        std::cerr << "parser error: EndTag: invalid element name\n";
        exit(1);
    }
    [[maybe_unused]] const std::string_view prefix(qName.substr(0, colonPosition));
    [[maybe_unused]] const std::string_view localName(qName.substr(colonPosition ? colonPosition + 1 : 0));
    TRACE("END TAG", "qName", qName, "prefix", prefix, "localName", localName);
    content.remove_prefix(nameEndPosition);
    content.remove_prefix(content.find_first_not_of(WHITESPACE));
    assert(content.compare(0, ">"sv.size(), ">"sv) == 0);
    content.remove_prefix(">"sv.size());
}
