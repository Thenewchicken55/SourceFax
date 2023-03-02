#include "XMLParser.hpp"
#include "xml_parser.hpp"
#include <bitset>

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

// get qName
std::string_view XMLParser::getQName() {
    return qName;
}

// set qName
void XMLParser::setQName(std::string_view newQName) {
    qName = newQName;
}

// get prefix
std::string_view XMLParser::getPrefix() {
    return prefix;
}

// get localName
std::string_view XMLParser::getLocalName() {
    return localName;
}

// get inEscape
bool XMLParser::inEscape() {
    // provides literal string operator""sv
    using namespace std::literals::string_view_literals;
    return (localName == "escape"sv);
}

// get characters
std::string_view XMLParser::getCharacters() {
    return characters;
}

// set characters
void XMLParser::setCharacters(std::string_view newCharacters) {
    characters = newCharacters;
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
void XMLParser::parseCharacterNotEntityReference() {
    characters = xml_parser::parseCharacterNotEntityReference(content);
}

// parse XML comment
int XMLParser::parseComment() {
    return xml_parser::parseComment(content, doneReading);
}

// parse CDATA
int XMLParser::parseCDATA() {
    const auto charactersAndTotalBytes = xml_parser::parseCDATA(content, doneReading);
    characters = charactersAndTotalBytes.second;
    return charactersAndTotalBytes.first;
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
void XMLParser::parseStartTag() {
    auto nameEndPosition = xml_parser::parseStartTag(content, qName, prefix, localName);
    content.remove_prefix(nameEndPosition);
}

// parse XML namespace
void XMLParser::parseNamespace() {
    xml_parser::parseNamespace(content);
}

// parse attribute
std::string_view XMLParser::parseAttribute() {
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
int XMLParser::sizeOfContent() {
    return content.size();
}

// wrapper for .npos
size_t XMLParser::npos() {
    return content.npos;
}

// wrapper for compare()
int XMLParser::compareContent(int position, int count, std::string string) {
    return content.compare(position, count, string);
}


bool XMLParser::isMatchNameMask() {
    const std::bitset<128> xmlNameMask("00000111111111111111111111111110100001111111111111111111111111100000001111111111011000000000000000000000000000000000000000000000");
    return xmlNameMask[content[0]];
}
