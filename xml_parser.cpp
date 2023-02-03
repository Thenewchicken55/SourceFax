/*
    xml_parser.cpp

    Implementation file for low-level XML parse functions
*/

#include "xml_parser.hpp"
#include <string_view>
#include <cassert>

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
