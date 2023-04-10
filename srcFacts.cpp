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
#include "srcFactsParser.hpp"

// provides literal string operator""sv
using namespace std::literals::string_view_literals;

int main(int argc, char* argv[]) {

    const auto startTime = std::chrono::steady_clock::now();
    std::string_view content;
    srcFactsParser parser = srcFactsParser(content);

    // parse XML
    parser.parse();

    const auto finishTime = std::chrono::steady_clock::now();
    const auto elapsedSeconds = std::chrono::duration_cast<std::chrono::duration<double>>(finishTime - startTime).count();
    const auto MLOCPerSecond = parser.getLoc() / elapsedSeconds / 1000000;
    const auto files = std::max(parser.getUnitCount() - 1, 1);
    std::cout.imbue(std::locale{""});
    const auto valueWidth = std::max(5, static_cast<int>(log10(parser.getTotalBytes()) * 1.3 + 1));
    std::cout << "# srcFacts: " << parser.getUrl() << '\n';
    std::cout << "| Measure      | " << std::setw(valueWidth + 3) << "Value |\n";
    std::cout << "|:-------------|-" << std::setw(valueWidth + 3) << std::setfill('-') << ":|\n" << std::setfill(' ');
    std::cout << "| Characters   | " << std::setw(valueWidth) << parser.getTextSize()        << " |\n";
    std::cout << "| LOC          | " << std::setw(valueWidth) << parser.getLoc()             << " |\n";
    std::cout << "| Files        | " << std::setw(valueWidth) << files                       << " |\n";
    std::cout << "| Classes      | " << std::setw(valueWidth) << parser.getClassCount()      << " |\n";
    std::cout << "| Functions    | " << std::setw(valueWidth) << parser.getFunctionCount()   << " |\n";
    std::cout << "| Declarations | " << std::setw(valueWidth) << parser.getDeclCount()       << " |\n";
    std::cout << "| Expressions  | " << std::setw(valueWidth) << parser.getExprCount()       << " |\n";
    std::cout << "| Comments     | " << std::setw(valueWidth) << parser.getCommentCount()    << " |\n";
    std::cout << "| Returns      | " << std::setw(valueWidth) << parser.getReturnCount()     << " |\n";
    std::cout << "| Line Comments| " << std::setw(valueWidth) << parser.getLineCommentCount()<< " |\n";
    std::cout << "| Strings      | " << std::setw(valueWidth) << parser.getStringCount()     << " |\n";
    std::clog.imbue(std::locale{""});
    std::clog.precision(3);
    std::clog << '\n';
    std::clog << parser.getTotalBytes()  << " bytes\n";
    std::clog << elapsedSeconds << " sec\n";
    std::clog << MLOCPerSecond << " MLOC/sec\n";

    return 0;
}
