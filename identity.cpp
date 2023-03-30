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

// provides literal string operator""sv
using namespace std::literals::string_view_literals;

constexpr auto WHITESPACE = " \n\t\r"sv;
[[maybe_unused]] constexpr auto NAMEEND = "> /\":=\n\t\r"sv;

std::string escape(std::string_view unescaped) {
    std::string unescapedString(unescaped);
    std::string escapedString = "";
    for(auto c: unescapedString) {
        if(c == '<') {
            escapedString += "&lt";
        } else if (c == '>') {
            escapedString += "&gt";
        } else if (c == '&') {
            escapedString += "&amp";
        } else {
            escapedString += c;
        }
    }
    return escapedString;
}

int main() {
    const auto startTime = std::chrono::steady_clock::now();
    int unitCount = 0;
    int loc = 0;
    std::string_view content;
    std::string url;
    XMLParser parser = XMLParser(content);

    const auto XMLDeclarationHandler =
        [&](std::string_view version, std::optional<std::string_view>& encoding, std::optional<std::string_view>& standalone)->void {
            std::cout << "<?xml version=\"" << version << "\" ";

            if (encoding.has_value()) {
                std::cout << "encoding=\"" << encoding.value() << "\" ";
            }

            if (standalone.has_value()) {
                std::cout << "standalone=\"" << standalone.value() << "\" ";
            }

            std::cout << "?>" << std::endl;
         };

    const auto startTagHandler =
    [&](std::string_view qName, std::string_view prefix, std::string_view localName)->void {
        std::cout << "<" << qName;

        if (localName == "unit"sv) {
            ++unitCount;
        }
    };

    const auto endTagHandler =
        [&](std::string_view qName, std::string_view prefix, std::string_view localName)->void {
            std::cout << "</" <<  qName << ">" << std::endl;
         };

    const auto characterEntityReferencesHandler =
        [&](std::string_view characters)->void {
            std::cout << escape(characters) << std::endl;
         };

    const auto characterNonEntityReferencesHandler =
    [&](std::string_view characters)->void {
        std::cout << escape(characters) << " ";

        loc += static_cast<int>(std::count(characters.cbegin(), characters.cend(), '\n'));
    };

    const auto attributeHandler =
    [&](std::string_view qName, std::string_view prefix, std::string_view localName, std::string_view value)->void {
        std::cout << " " << qName << "=\"" << value << "\">";

        if (localName == "url"sv) {
            url = value;
        }
    };

    const auto XMLNamespaceHandler =
        [&](std::string_view prefix, std::string_view uri)->void {
            std::cout << " xmlns";
            if (!prefix.empty()) {
                std::cout << ":" << prefix;
            }
            std::cout << "=\"" << uri << "\"";
         };

    const auto XMLCommentHandler =
        [&](std::string_view value)->void {
            std::cout << "<!--" << value << "-->" << std::endl;
        };

    const auto CDATAHandler =
    [&](std::string_view characters)->void {
        std::cout << "<![CDATA[" << escape(characters) << "]]>";

        loc += static_cast<int>(std::count(characters.cbegin(), characters.cend(), '\n'));
    };

    const auto processingInstructionHandler =
        [&](std::string_view target, std::string_view data)->void {
            std::cout << "<?" << target << " " << data << "?>" << std::endl;
         };

    // parse XML
    parser.parse(

        // null Start Document handler
        nullptr,

        // null XML declaration handler
        XMLDeclarationHandler,

        // start tag handler
        startTagHandler,

        // null End tag handler
        endTagHandler,

        // character entity references handler
        characterEntityReferencesHandler,

        // character non-entity references handler
        characterNonEntityReferencesHandler,

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
        nullptr

        );

    const auto finishTime = std::chrono::steady_clock::now();
    const auto elapsedSeconds = std::chrono::duration_cast<std::chrono::duration<double>>(finishTime - startTime).count();
    const auto MLOCPerSecond = loc / elapsedSeconds / 1000000;
    std::cout.imbue(std::locale{""});
    std::clog.imbue(std::locale{""});
    std::clog.precision(3);
    std::clog << '\n';
    std::clog << parser.getTotalBytes()  << " bytes\n";
    std::clog << elapsedSeconds << " sec\n";
    std::clog << MLOCPerSecond << " MLOC/sec\n";
    return 0;
}