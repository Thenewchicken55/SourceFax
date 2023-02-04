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
void refillPreserve(std::string_view& content, bool& doneReading, long& totalBytes);

// parse character entity references
void parseCharER(std::string_view& content);

// parse character non-entity references
void parseCharNonER(std::string_view& content, int& loc, int& textSize);

// parse XML comment
void parseComment(std::string_view& content, bool& doneReading, long& totalBytes);

#endif
