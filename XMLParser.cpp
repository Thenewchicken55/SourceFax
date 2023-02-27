#include "XMLParser.hpp"
#include "xml_parser.hpp"

// constructor
XMLParser::XMLParser() {
    
}

// parse file from the start
int XMLParser::parseBegin(std::string_view& content) {
    return xml_parser::parseBegin(content);
}

// parse XML declaration
void XMLParser::parseXMLDeclaration(std::string_view content) {
    xml_parser::parseXMLDeclaration(content);

}

//parse DOCTYPE
void XMLParser::parseDOCTYPE(std::string_view& content) {
    xml_parser::parseDOCTYPE(content);
}

// refill content preserving unprocessed
int XMLParser::refillPreserve(std::string_view& content, bool& doneReading) {
    return xml_parser::refillPreserve(content, doneReading);
}

// parse character entity references
void XMLParser::parseCharacterEntityReference(std::string_view& content) {
    xml_parser::parseCharacterEntityReference(content);
}

// parse character non-entity references
std::string_view XMLParser::parseCharacterNotEntityReference(std::string_view& content) {
    return xml_parser::parseCharacterNotEntityReference(content);
}

// parse XML comment
int XMLParser::parseComment(std::string_view& content, bool& doneReading) {
    return xml_parser::parseComment(content, doneReading);
}

// parse CDATA
std::pair<int, std::string_view> XMLParser::parseCDATA(std::string_view& content, bool& doneReading) {
    return xml_parser::parseCDATA(content, doneReading);
}

// parse processing instruction
void XMLParser::parseProcessing(std::string_view& content) {
    xml_parser::parseProcessing(content);
}

// parse end tag
void XMLParser::parseEndTag(std::string_view& content) {
    xml_parser::parseEndTag(content);
}

// parse start tag
std::size_t XMLParser::parseStartTag(std::string_view& content, std::string_view& qName, std::string_view& prefix, std::string_view& localName) {
    return xml_parser::parseStartTag(content, qName, prefix, localName);
}

// parse XML namespace
void XMLParser::parseNamespace(std::string_view& content) {
    xml_parser::parseNamespace(content);
}

// parse attribute
std::size_t XMLParser::parseAttribute(std::string_view& content) {
    return xml_parser::parseAttribute(content);
}

// Accessor::predicate to test if the tag is a XML declaration
bool XMLParser::isXML(std::string_view content) {
    return xml_parser::isXML(content);
}

// Accessor::predicate to test if the tag is DOCTYPE
bool XMLParser::isDOCTYPE(std::string_view content) {
    return xml_parser::isDOCTYPE(content);
}

// Accessor::predicate to test if the tag is CDATA
bool XMLParser::isCDATA(std::string_view content) {
    return xml_parser::isCDATA(content);
}

// Accessor::predicate to test if the tag is a comment tag
bool XMLParser::isComment(std::string_view content) {
    return xml_parser::isComment(content);
}

// Accessor::predicate to test if the tag is an XML namespace
bool XMLParser::isNamespace(std::string_view content) {
    return xml_parser::isNamespace(content);
}

// Accessor::predicate to check if content has a specific character at a specific index
bool XMLParser::isCharacter(std::string_view& content, int index, char character) {
    return xml_parser::isCharacter(content, index, character);
}