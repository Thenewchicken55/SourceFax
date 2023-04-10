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
#include "XMLStatsParser.hpp"

// provides literal string operator""sv
using namespace std::literals::string_view_literals;

int main() {
    const auto startTime = std::chrono::steady_clock::now();
    std::string_view content;
    XMLStatsParser parser = XMLStatsParser(content);

    // parse XML
    parser.parse();

    const auto finishTime = std::chrono::steady_clock::now();
    const auto elapsedSeconds = std::chrono::duration_cast<std::chrono::duration<double>>(finishTime - startTime).count();
    const auto MLOCPerSecond = parser.getLoc() / elapsedSeconds / 1000000;
    std::cout.imbue(std::locale{""});
    const auto valueWidth = std::max(5, static_cast<int>(log10(parser.getTotalBytes()) * 1.3 + 1));
    std::cout << "# xmlStats: " << parser.getUrl() << '\n';
    std::cout << "| Measure                 | " << std::setw(valueWidth + 3) << "Value |\n";
    std::cout << "|:------------------------|-" << std::setw(valueWidth + 3) << std::setfill('-')       << ":|\n" << std::setfill(' ');
    std::cout << "| Start Document          | " << std::setw(valueWidth) << parser.getStartDocumentCount()          << " |\n";
    std::cout << "| XML Declarations        | " << std::setw(valueWidth) << parser.getXMLDeclarationCount()         << " |\n";
    std::cout << "| Start Tags              | " << std::setw(valueWidth) << parser.getStartTagCount()               << " |\n";
    std::cout << "| End Tags                | " << std::setw(valueWidth) << parser.getEndTagCount()                 << " |\n";
    std::cout << "| Characters              | " << std::setw(valueWidth) << parser.getCharactersCount()             << " |\n";
    std::cout << "| Attributes              | " << std::setw(valueWidth) << parser.getAttributeCount()              << " |\n";
    std::cout << "| XML Namespaces          | " << std::setw(valueWidth) << parser.getXMLNamespaceCount()           << " |\n";
    std::cout << "| XML Comments            | " << std::setw(valueWidth) << parser.getXMLCommentCount()             << " |\n";
    std::cout << "| CDATA                   | " << std::setw(valueWidth) << parser.getCDATACount()                  << " |\n";
    std::cout << "| Processing Instructions | " << std::setw(valueWidth) << parser.getProcessingInstructionCount()  << " |\n";
    std::cout << "| End Document            | " << std::setw(valueWidth) << parser.getEndDocumentCount()            << " |\n";
    std::clog.imbue(std::locale{""});
    std::clog.precision(3);
    std::clog << '\n';
    std::clog << parser.getTotalBytes()  << " bytes\n";
    std::clog << elapsedSeconds << " sec\n";
    std::clog << MLOCPerSecond << " MLOC/sec\n";
    return 0;
}
