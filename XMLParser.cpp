#include "XMLParser.hpp"
#include "xml_parser.hpp"

// constructor
XMLParser::XMLParser(std::string_view content)
    : content(content)
    {}

// get doneReading
bool XMLParser::isDoneReading() {
    return doneReading;
}

// set doneReading
// @param[in] change doneReading to newValue
void XMLParser::setDoneReading(bool newBool) {
    doneReading = newBool;
}

// parse file from the start
int XMLParser::parseBegin() {
    return xml_parser::parseBegin(content);
}

// parse XML declaration
void XMLParser::parseXMLDeclaration() {
    xml_parser::parseXMLDeclaration(content);

}

//parse DOCTYPE
void XMLParser::parseDOCTYPE() {
    xml_parser::parseDOCTYPE(content);
}

// refill content preserving unprocessed
int XMLParser::refillPreserve() {
    return xml_parser::refillPreserve(content, doneReading);
}

// parse character entity references
void XMLParser::parseCharacterEntityReference() {
    xml_parser::parseCharacterEntityReference(content);
}

// parse character non-entity references
std::string_view XMLParser::parseCharacterNotEntityReference() {
    return xml_parser::parseCharacterNotEntityReference(content);
}

// parse XML comment
int XMLParser::parseComment() {
    return xml_parser::parseComment(content, doneReading);
}

// parse CDATA
std::pair<int, std::string_view> XMLParser::parseCDATA() {
    return xml_parser::parseCDATA(content, doneReading);
}

// parse processing instruction
void XMLParser::parseProcessing() {
    xml_parser::parseProcessing(content);
}

// parse end tag
void XMLParser::parseEndTag() {
    xml_parser::parseEndTag(content);
}

// parse start tag
std::size_t XMLParser::parseStartTag(std::string_view& qName, std::string_view& prefix, std::string_view& localName) {
    return xml_parser::parseStartTag(content, qName, prefix, localName);
}

// parse XML namespace
void XMLParser::parseNamespace() {
    xml_parser::parseNamespace(content);
}

// parse attribute
std::size_t XMLParser::parseAttribute() {
    return xml_parser::parseAttribute(content);
}

// Accessor::predicate to test if the tag is a XML declaration
bool XMLParser::isXML() {
    return xml_parser::isXML(content);
}

// Accessor::predicate to test if the tag is DOCTYPE
bool XMLParser::isDOCTYPE() {
    return xml_parser::isDOCTYPE(content);
}

// Accessor::predicate to test if the tag is CDATA
bool XMLParser::isCDATA() {
    return xml_parser::isCDATA(content);
}

// Accessor::predicate to test if the tag is a comment tag
bool XMLParser::isComment() {
    return xml_parser::isComment(content);
}

// Accessor::predicate to test if the tag is an XML namespace
bool XMLParser::isNamespace() {
    return xml_parser::isNamespace(content);
}

// Accessor::predicate to check if content has a specific character at a specific index
bool XMLParser::isCharacter(int index, char character) {
    return xml_parser::isCharacter(content, index, character);
}

// wrapper for remove_prefix()
void XMLParser::removePrefix(int index) {
    content.remove_prefix(index);
}

// wrapper for find_first_not_of()
std::size_t XMLParser::findFirstNotOf(std::string_view string) {
    return content.find_first_not_of(string);
}

// wrapper for size()
int XMLParser::getSizeOfContent() {
    return content.size();
}

// wrapper for .npos
size_t XMLParser::npos() {
    return content.npos;
}

// @return content[0]  
char XMLParser::getFirstCharFromContent() {
    return content[0];
}

// wrapper for substr()
std::string_view XMLParser::subString(int begin, int end) {
    return content.substr(begin, end);
}

// wrapper for compare()
int XMLParser::compareContent(int position, int count, std::string string) {
    return content.compare(position, count, string);
}