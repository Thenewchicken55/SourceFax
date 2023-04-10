/*
    XMLStatsParser.cpp

    Implementation file for the concrete class specific to XML Stats inheriting from the abstract class XMLParser
*/

#include "XMLStatsParser.hpp"

// constructor
XMLStatsParser::XMLStatsParser(std::string_view content) 
: XMLParser(content), unitCount(0), loc(0),
startDocumentCount(0), XMLDeclarationCount(0),
startTagCount(0), endTagCount(0), charactersCount(0),
attributeCount(0), XMLNamespaceCount(0),
XMLCommentCount(0), CDATACount(0),
processingInstructionCount(0), endDocumentCount(0)
{}

// get unitCount
int XMLStatsParser::getUnitCount()
{
    return unitCount;
}

// get loc
int XMLStatsParser::getLoc()
{
    return loc;
}

// get startDocumentCount
int XMLStatsParser::getStartDocumentCount()
{
    return startDocumentCount;
}

// get XMLDeclarationCount
int XMLStatsParser::getXMLDeclarationCount()
{
    return XMLDeclarationCount;
}

// get startTagCount
int XMLStatsParser::getStartTagCount()
{
    return startTagCount;
}

// get endTagCount
int XMLStatsParser::getEndTagCount()
{
    return endTagCount;
}

// get charactersCount
int XMLStatsParser::getCharactersCount()
{
    return charactersCount;
}

// get attributeCount
int XMLStatsParser::getAttributeCount()
{
    return attributeCount;
}

// get XMLNamespaceCount
int XMLStatsParser::getXMLNamespaceCount()
{
    return XMLNamespaceCount;
}

// get XMLCommentCount
int XMLStatsParser::getXMLCommentCount()
{
    return XMLCommentCount;
}

// get CDATACount
int XMLStatsParser::getCDATACount()
{
    return CDATACount;
}

// get processingInstructionCount
int XMLStatsParser::getProcessingInstructionCount()
{
    return processingInstructionCount;
}

// get endDocumentCount
int XMLStatsParser::getEndDocumentCount()
{
    return endDocumentCount;
}

// start Document Handler
void XMLStatsParser::handleStartDocument() {
    ++startDocumentCount;
}

// XML Declaration Handler
void XMLStatsParser::handleXMLDeclaration(std::string_view version, std::optional<std::string_view>& encoding, std::optional<std::string_view>& standalone) {
    ++XMLDeclarationCount;
}

// Start Tag Handler
void XMLStatsParser::handleStartTag(std::string_view qName, std::string_view prefix, std::string_view localName) {
    ++startTagCount;

    if (localName == "unit"sv) {
        ++unitCount;
    }
}

// End Tag Handler
void XMLStatsParser::handleEndTag(std::string_view qName, std::string_view prefix, std::string_view localName) {
    ++endTagCount;
}

// Character Handler
void XMLStatsParser::handleCharacter(std::string_view characters) {
    ++charactersCount;

    loc += static_cast<int>(std::count(characters.cbegin(), characters.cend(), '\n'));
}

// attribute Handler
void XMLStatsParser::handleAttribute(std::string_view qName, std::string_view prefix, std::string_view localName, std::string_view value) {
    ++attributeCount;
}

// XML Namespace Handler
void XMLStatsParser::handleXMLNamespace(std::string_view prefix, std::string_view uri) {
    ++XMLNamespaceCount;
}

// XML Comment Handler
void XMLStatsParser::handleXMLComment(std::string_view value) {
    ++XMLCommentCount;
}

// CDATA Handler
void XMLStatsParser::handleCDATA(std::string_view characters) {
    ++CDATACount;

    loc += static_cast<int>(std::count(characters.cbegin(), characters.cend(), '\n'));
}

// processing Instruction Handler
void XMLStatsParser::handleProcessingInstruction(std::string_view target, std::string_view data) {
    ++processingInstructionCount;
}

// end Document Handler
void XMLStatsParser::handleEndDocument() {
    ++endDocumentCount;
}
