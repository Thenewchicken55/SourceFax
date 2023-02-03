/*
    xml_parser.hpp

    Include file for low-level XML parse functions
*/

#ifndef INCLUDED_XML_PARSER_HPP
#define INCLUDED_XML_PARSER_HPP

#include <string_view>

// parse XML declaration
void parseXMLDeclaration(std::string_view& content);

#endif