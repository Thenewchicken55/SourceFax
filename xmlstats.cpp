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
    std::string_view content;
    std::string url;
    XMLParser parser = XMLParser(content);


    // parse XML
    parser.parse(
        
        // null Start Document handler
        nullptr, 

        // null XML declaration handler
        nullptr,

        // start tag handler
        nullptr,

        // null End tag handler
        nullptr,

        // character entity references handler
        nullptr,

        // character non-entity references handler
        nullptr,

        // attribute handler
        nullptr,

        // null XML namespace handler
        nullptr,

        // null XML Comment handler
        nullptr,
        
        // CDATA handler
        nullptr,

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
    std::cout << "| LOC          | " << std::setw(valueWidth) << loc             << " |\n";
    std::cout << "| Files        | " << std::setw(valueWidth) << files           << " |\n";
    std::clog.imbue(std::locale{""});
    std::clog.precision(3);
    std::clog << '\n';
    std::clog << parser.getTotalBytes()  << " bytes\n";
    std::clog << elapsedSeconds << " sec\n";
    std::clog << MLOCPerSecond << " MLOC/sec\n";
    return 0;
}
