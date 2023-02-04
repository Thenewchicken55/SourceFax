/*
    xml_parser.hpp

    Include file for low-level XML parse functions
*/

#ifndef INCLUDED_XML_PARSER_HPP
#define INCLUDED_XML_PARSER_HPP

#include <string_view>

// parse XML declaration
void parseXMLDeclaration(std::string_view& content);

// parse required version
void parseVersion(std::string_view& content);

// parse optional encoding and standalone attributes
void parseOptional(std::string_view& content);

//parse DOCTYPE
void parseDOCTYPE(std::string_view& content);
// refill content preserving unprocessed
void refillPreserve(std::string_view& content, bool& doneReading);

#endif
