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
#include <algorithm>
#include <iomanip>
#include <string_view>
#include <cmath>
#include <chrono>
#include <cassert>
#include "refillContent.hpp"
#include "XMLParser.hpp"

// provides literal string operator""sv
using namespace std::literals::string_view_literals;

constexpr auto WHITESPACE = " \n\t\r"sv;
[[maybe_unused]] constexpr auto NAMEEND = "> /\":=\n\t\r"sv;

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
    std::string_view content;
    XMLParser parser = XMLParser(content);

    auto startTagHandler =
    [&](std::string_view& qName, std::string_view& prefix, std::string_view& localName)->void {
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
    };

    auto characterEntityReferencesHandler =
    [&textSize](std::string_view& characters)->void {
        ++textSize;
    };

    auto characterNonEntityReferencesHandler =
    [&loc, &textSize](std::string_view& characters)->void {
        loc += static_cast<int>(std::count(characters.cbegin(), characters.cend(), '\n'));
        textSize += static_cast<int>(characters.size());
    };

    auto attributeHandler =
    [&url, &stringCount, &lineCommentCount](std::string_view& qName, std::string_view& prefix, std::string_view& localName, std::string_view& value)->void {
        if (localName == "url"sv) {
            url = value;
        } else if (localName == "literal"sv && value == "string"sv) {
            ++stringCount;
        } else if (localName == "comment"sv && value == "line") {
            ++lineCommentCount;
        }
    };

    auto CDATAHandler =
    [&textSize, &loc](std::string_view& characters)->void {
        textSize += static_cast<int>(characters.size());
        loc += static_cast<int>(std::count(characters.cbegin(), characters.cend(), '\n'));
    };

    // parse XML
    parser.parse(

        // null Start Document handler
        nullptr,

        // null XML declaration handler
        nullptr,

        // start tag handler
        startTagHandler,

        // null End tag handler
        nullptr,

        // character entity references handler
        characterEntityReferencesHandler,

        // character non-entity references handler
        characterNonEntityReferencesHandler,

        // attribute handler
        attributeHandler,

        // null XML namespace handler
        nullptr,

        // null XML Comment handler
        nullptr,

        // CDATA handler
        CDATAHandler,

        // null processing instruction handler
        nullptr,

        // null end document handler
        nullptr

        );

    const auto finishTime = std::chrono::steady_clock::now();
    const auto elapsedSeconds = std::chrono::duration_cast<std::chrono::duration<double>>(finishTime - startTime).count();
    const auto MLOCPerSecond = loc / elapsedSeconds / 1000000;
    const auto files = std::max(unitCount - 1, 1);
    std::cout.imbue(std::locale{""});
    const auto valueWidth = std::max(5, static_cast<int>(log10(parser.getTotalBytes()) * 1.3 + 1));
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
    std::clog << parser.getTotalBytes()  << " bytes\n";
    std::clog << elapsedSeconds << " sec\n";
    std::clog << MLOCPerSecond << " MLOC/sec\n";

    return 0;
}
