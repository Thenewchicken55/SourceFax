/*
    xml_parser.cpp

    Implementation file for low-level XML parse functions
*/

#include "xml_parser.hpp"
#include <string_view>
#include <cassert>
#include <iostream>
#include <optional>

#define TRACE(...)

// provides literal string operator""sv
using namespace std::literals::string_view_literals;

constexpr auto WHITESPACE = " \n\t\r"sv;

// parse XML declaration
void parseXMLDeclaration(std::string_view& content)
{
    assert(content.compare(0, "<?xml "sv.size(), "<?xml "sv) == 0);
    content.remove_prefix("<?xml"sv.size());
    content.remove_prefix(content.find_first_not_of(WHITESPACE));
}

// parse required version
void parseVersion(std::string_view& content){
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

std::optional<std::string_view> encoding;
std::optional<std::string_view> standalone;

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
