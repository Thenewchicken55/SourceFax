/*
    xmlstats.cpp

    Create the application xmlstats that produces a markdown
    report (similar to what srcFacts does) that indicates the
    number of each part of XML. E.g., the number of start
    tags, end tags, attributes, character sections, etc.

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

int main() {
    const auto startTime = std::chrono::steady_clock::now();
    int unitCount = 0;
    int loc = 0;
    std::string url;
    int startDocumentCount = 0;
    int XMLDeclarationCount = 0;
    int startTagCount = 0;
    int endTagCount = 0;
    int charactersCount = 0;
    int attributeCount = 0;
    int XMLNamespaceCount = 0;
    int XMLCommentCount = 0;
    int CDATACount = 0;
    int processingInstructionCount = 0;
    int endDocumentCount = 0;
    std::string_view content;
    XMLParser parser = XMLParser(content);

    const auto startDocumentHandler =
    [&]()->void {
        ++startDocumentCount;
    };

    const auto XMLDeclarationHandler =
    [&](std::string_view version, std::optional<std::string_view>& encoding, std::optional<std::string_view>& standalone)->void {
        ++XMLDeclarationCount;
    };

    const auto startTagHandler =
    [&](std::string_view qName, std::string_view prefix, std::string_view localName)->void {
        ++startTagCount;

        if (localName == "unit"sv) {
            ++unitCount;
        }
    };

    const auto endTagHandler =
    [&](std::string_view qName, std::string_view prefix, std::string_view localName)->void {
        ++endTagCount;
    };

    const auto characterHandler =
    [&](std::string_view characters)->void {
        ++charactersCount;

        loc += static_cast<int>(std::count(characters.cbegin(), characters.cend(), '\n'));
    };

    const auto attributeHandler =
    [&](std::string_view qName, std::string_view prefix, std::string_view localName, std::string_view value)->void {
        ++attributeCount;

        if (localName == "url"sv) {
            url = value;
        }
    };

    const auto XMLNamespaceHandler =
    [&](std::string_view prefix, std::string_view uri)->void {
        ++XMLNamespaceCount;
    };

    const auto XMLCommentHandler =
    [&](std::string_view value)->void {
        ++XMLCommentCount;
    };

    const auto CDATAHandler =
    [&](std::string_view characters)->void {
        ++CDATACount;

        loc += static_cast<int>(std::count(characters.cbegin(), characters.cend(), '\n'));
    };

    const auto processingInstructionHandler =
    [&](std::string_view target, std::string_view data)->void {
        ++processingInstructionCount;
    };

    const auto endDocumentHandler =
    [&]()->void {
        ++endDocumentCount;
    };

    // parse XML
    parser.parse(

        // null Start Document handler
        startDocumentHandler,

        // null XML declaration handler
        XMLDeclarationHandler,

        // start tag handler
        startTagHandler,

        // null End tag handler
        endTagHandler,

        // character handler
        characterHandler,

        // attribute handler
        attributeHandler,

        // null XML namespace handler
        XMLNamespaceHandler,

        // null XML Comment handler
        XMLCommentHandler,

        // CDATA handler
        CDATAHandler,

        // null processing instruction handler
        processingInstructionHandler,

        // null end document handler
        endDocumentHandler
        );

    const auto finishTime = std::chrono::steady_clock::now();
    const auto elapsedSeconds = std::chrono::duration_cast<std::chrono::duration<double>>(finishTime - startTime).count();
    const auto MLOCPerSecond = loc / elapsedSeconds / 1000000;
    std::cout.imbue(std::locale{""});
    const auto valueWidth = std::max(5, static_cast<int>(log10(parser.getTotalBytes()) * 1.3 + 1));
    std::cout << "# xmlStats: " << url << '\n';
    std::cout << "| Measure                 | " << std::setw(valueWidth + 3) << "Value |\n";
    std::cout << "|:------------------------|-" << std::setw(valueWidth + 3) << std::setfill('-')       << ":|\n" << std::setfill(' ');
    std::cout << "| Start Document          | " << std::setw(valueWidth) << startDocumentCount          << " |\n";
    std::cout << "| XML Declarations        | " << std::setw(valueWidth) << XMLDeclarationCount         << " |\n";
    std::cout << "| Start Tags              | " << std::setw(valueWidth) << startTagCount               << " |\n";
    std::cout << "| End Tags                | " << std::setw(valueWidth) << endTagCount                 << " |\n";
    std::cout << "| Characters              | " << std::setw(valueWidth) << charactersCount             << " |\n";
    std::cout << "| Attributes              | " << std::setw(valueWidth) << attributeCount              << " |\n";
    std::cout << "| XML Namespaces          | " << std::setw(valueWidth) << XMLNamespaceCount           << " |\n";
    std::cout << "| XML Comments            | " << std::setw(valueWidth) << XMLCommentCount             << " |\n";
    std::cout << "| CDATA                   | " << std::setw(valueWidth) << CDATACount                  << " |\n";
    std::cout << "| Processing Instructions | " << std::setw(valueWidth) << processingInstructionCount  << " |\n";
    std::cout << "| End Document            | " << std::setw(valueWidth) << endDocumentCount            << " |\n";
    std::clog.imbue(std::locale{""});
    std::clog.precision(3);
    std::clog << '\n';
    std::clog << parser.getTotalBytes()  << " bytes\n";
    std::clog << elapsedSeconds << " sec\n";
    std::clog << MLOCPerSecond << " MLOC/sec\n";
    return 0;
}
