/*
    identity.cpp

    Create the application identity which is an identity
    transformation. A transformation is a program that converts
    data in one format to data in a related format, typically
    the same format. In this case, it converts from XML to XML.
    It is an identity transformation because the output XML
    should be the same (as much as possible) as the input XML.
    The XMLParser will parse the input, and your callbacks will
    output the XML format. You can assume that there are no
    CDATA parts, but you must escape all >, <, and & in
    Character and CDATA content.

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
#include "IdentityHandler.hpp"

// provides literal string operator""sv
using namespace std::literals::string_view_literals;

int main() {
    const auto startTime = std::chrono::steady_clock::now();
    std::string_view content;
    IdentityHandler handler;
    XMLParser parser(content, handler);

    // parse XML
    parser.parse();

    const auto finishTime = std::chrono::steady_clock::now();
    const auto elapsedSeconds = std::chrono::duration_cast<std::chrono::duration<double>>(finishTime - startTime).count();
    const auto MLOCPerSecond = handler.getLoc() / elapsedSeconds / 1000000;
    std::clog.imbue(std::locale{""});
    std::clog.precision(3);
    std::clog << '\n';
    std::clog << parser.getTotalBytes()  << " bytes\n";
    std::clog << elapsedSeconds << " sec\n";
    std::clog << MLOCPerSecond << " MLOC/sec\n";

    return 0;
}
