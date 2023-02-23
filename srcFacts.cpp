/*
    srcFacts.cpp

    Produces a report with various measures of source code.
    Supports C++, C, Java, and C#. Input is an XML file in the srcML format,
    and output is a markdown table with the measures. Performance statistics
    are output to standard error.

    The code includes a complete XML parser:
    * Characters and content from XML is in UTF-8
    * DTD declarations are allowed, but not fine-grained parsed
    * No checking for well-formedness
*/

#include <iostream>
#include <locale>
#include <iterator>
#include <string>
#include <algorithm>
#include <sys/types.h>
#include <errno.h>
#include <string_view>
#include <optional>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <memory>
#include <stdlib.h>
#include <bitset>
#include <cassert>
#include <utility>
#include "refillContent.hpp"
#include "xml_parser.hpp"

// provides literal string operator""sv
using namespace std::literals::string_view_literals;

const int BLOCK_SIZE = 4096;

const std::bitset<128> xmlNameMask("00000111111111111111111111111110100001111111111111111111111111100000001111111111011000000000000000000000000000000000000000000000");

constexpr auto WHITESPACE = " \n\t\r"sv;
[[maybe_unused]] constexpr auto NAMEEND = "> /\":=\n\t\r"sv;

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

int main(int argc, char* argv[]) {

    const auto startTime = std::chrono::steady_clock::now();
    std::string url;
    int textSize = 0;
    int loc = 0;
    int exprCount = 0;
    int functionCount = 0;
    int classCount = 0;
    int unitCount = 0;
    int declCount = 0;
    int commentCount = 0;
    int returnCount = 0;
    int lineCommentCount = 0;
    int stringCount = 0;
    long totalBytes = 0;
    std::string_view content;

    // parse file from the start
    auto bytesRead = xml_parser::parseBegin(content);
    totalBytes += bytesRead;

    content.remove_prefix(content.find_first_not_of(WHITESPACE));
    if (xml_parser::isXML(content)) {
        // parse XML Declaration
        xml_parser::parseXMLDeclaration(content);
    }
    if (xml_parser::isDOCTYPE(content)) {
        // parse DOCTYPE
        xml_parser::parseDOCTYPE(content);
    }
    int depth = 0;
    bool doneReading = false;
    while (true) {
        if (doneReading) {
            if (content.empty())
                break;
        } else if (content.size() < BLOCK_SIZE) {
            // refill content preserving unprocessed
            bytesRead = xml_parser::refillPreserve(content, doneReading);
            totalBytes += bytesRead;
        }
        if (xml_parser::isCharacter(content, 0, '&')) {
            // parse character entity references
            xml_parser::parseCharacterEntityReference(content);
            ++textSize;
        } else if (!xml_parser::isCharacter(content, 0 ,'<')) {
            // parse character non-entity references
            auto characters = xml_parser::parseCharacterNotEntityReference(content);
            loc += static_cast<int>(std::count(characters.cbegin(), characters.cend(), '\n'));
            textSize += static_cast<int>(characters.size());
        } else if (xml_parser::isComment(content)) {
            // parse XML comment
            bytesRead = xml_parser::parseComment(content, doneReading);
            totalBytes += bytesRead;
            content.remove_prefix("-->"sv.size());
        } else if (xml_parser::isCDATA(content)) {
            // parse CDATA
            const auto result = xml_parser::parseCDATA(content, doneReading);
            totalBytes = result.first;
            const auto characters = result.second;
            totalBytes += bytesRead;
            textSize += static_cast<int>(characters.size());
            loc += static_cast<int>(std::count(characters.cbegin(), characters.cend(), '\n'));
        } else if (xml_parser::isCharacter(content, 1, '?') /* && xml_parser::isCharacter(content, 0, '<') */) {
            // parse processing instruction
            xml_parser::parseProcessing(content);
        } else if (xml_parser::isCharacter(content, 1, '/') /* && xml_parser::isCharacter(content, 0, '<') */) {
            // parse end tag
            xml_parser::parseEndTag(content);
            --depth;
            if (depth == 0)
                break;
        } else if (xml_parser::isCharacter(content, 0, '<')) {
            std::string_view qName;
            [[maybe_unused]] std::string_view prefix;
            std::string_view localName;
            // parse start tag
            const auto nameEndPosition = xml_parser::parseStartTag(content, qName, prefix, localName);
            const auto inEscape = localName == "escape"sv;
            if (localName == "expr"sv) {
                ++exprCount;
            } else if (localName == "decl"sv) {
                ++declCount;
            } else if (localName == "comment"sv) {
                ++commentCount;
            } else if (localName == "function"sv) {
                ++functionCount;
            } else if (localName == "unit"sv) {
                ++unitCount;
            } else if (localName == "class"sv) {
                ++classCount;
            } else if (localName == "return"sv) {
                ++returnCount;
            }
            content.remove_prefix(nameEndPosition);
            content.remove_prefix(content.find_first_not_of(WHITESPACE));
            while (xmlNameMask[content[0]]) {
                if (xml_parser::isNamespace(content)) {
                    // parse XML namespace
                    xml_parser::parseNamespace(content);
                } else {
                    // parse attribute
                    const auto valueEndPosition = xml_parser::parseAttribute(content);
                    const std::string_view value(content.substr(0, valueEndPosition));
                    if (localName == "url"sv)
                        url = value;
                    TRACE("ATTRIBUTE", "qName", qName, "prefix", prefix, "localName", localName, "value", value);
                    if (localName == "literal"sv && value == "string"sv) {
                        ++stringCount;
                    } else if (localName == "comment"sv && value == "line") {
                        ++lineCommentCount;
                    }
                    // convert special srcML escaped element to characters
                    if (inEscape && localName == "char"sv /* && inUnit */) {
                        // use strtol() instead of atoi() since strtol() understands hex encoding of '0x0?'
                        [[maybe_unused]] const auto escapeValue = (char)strtol(value.data(), NULL, 0);
                    }
                    content.remove_prefix(valueEndPosition);
                    content.remove_prefix("\""sv.size());
                    content.remove_prefix(content.find_first_not_of(WHITESPACE));
                }
            }
            if (xml_parser::isCharacter(content, 0, '>')) {
                content.remove_prefix(">"sv.size());
                ++depth;
            } else if (xml_parser::isCharacter(content, 0, '/') && xml_parser::isCharacter(content, 1, '>')) {
                assert(content.compare(0, "/>"sv.size(), "/>") == 0);
                content.remove_prefix("/>"sv.size());
                TRACE("END TAG", "qName", qName, "prefix", prefix, "localName", localName);
                if (depth == 0)
                    break;
            }
        } else {
            std::cerr << "parser error : invalid XML document\n";
            return 1;
        }
    }
    content.remove_prefix(content.find_first_not_of(WHITESPACE) == content.npos ? content.size() : content.find_first_not_of(WHITESPACE));
    while (xml_parser::isComment(content)) {
        // parse XML comment
        bytesRead = xml_parser::parseComment(content, doneReading);
        totalBytes += bytesRead;
    }
    if (!content.empty()) {
        std::cerr << "parser error : extra content at end of document\n";
        return 1;
    }
    TRACE("END DOCUMENT");
    const auto finishTime = std::chrono::steady_clock::now();
    const auto elapsedSeconds = std::chrono::duration_cast<std::chrono::duration<double>>(finishTime - startTime).count();
    const auto MLOCPerSecond = loc / elapsedSeconds / 1000000;
    const auto files = std::max(unitCount - 1, 1);
    std::cout.imbue(std::locale{""});
    const auto valueWidth = std::max(5, static_cast<int>(log10(totalBytes) * 1.3 + 1));
    std::cout << "# srcFacts: " << url << '\n';
    std::cout << "| Measure      | " << std::setw(valueWidth + 3) << "Value |\n";
    std::cout << "|:-------------|-" << std::setw(valueWidth + 3) << std::setfill('-') << ":|\n" << std::setfill(' ');
    std::cout << "| Characters   | " << std::setw(valueWidth) << textSize        << " |\n";
    std::cout << "| LOC          | " << std::setw(valueWidth) << loc             << " |\n";
    std::cout << "| Files        | " << std::setw(valueWidth) << files           << " |\n";
    std::cout << "| Classes      | " << std::setw(valueWidth) << classCount      << " |\n";
    std::cout << "| Functions    | " << std::setw(valueWidth) << functionCount   << " |\n";
    std::cout << "| Declarations | " << std::setw(valueWidth) << declCount       << " |\n";
    std::cout << "| Expressions  | " << std::setw(valueWidth) << exprCount       << " |\n";
    std::cout << "| Comments     | " << std::setw(valueWidth) << commentCount    << " |\n";
    std::cout << "| Returns      | " << std::setw(valueWidth) << returnCount     << " |\n";
    std::cout << "| Line Comments| " << std::setw(valueWidth) << lineCommentCount<< " |\n";
    std::cout << "| Strings      | " << std::setw(valueWidth) << stringCount     << " |\n";
    std::clog.imbue(std::locale{""});
    std::clog.precision(3);
    std::clog << '\n';
    std::clog << totalBytes  << " bytes\n";
    std::clog << elapsedSeconds << " sec\n";
    std::clog << MLOCPerSecond << " MLOC/sec\n";
    
    return 0;
}
