/*
    XMLStatsHandler.cpp

    Implementation file for the concrete class specific to XML Stats inheriting from the abstract class XMLParser
*/

#include "XMLStatsHandler.hpp"
#include <algorithm>

// constructor
XMLStatsHandler::XMLStatsHandler() :
    unitCount(0), loc(0),
    startDocumentCount(0), XMLDeclarationCount(0),
    startTagCount(0), endTagCount(0), charactersCount(0),
    attributeCount(0), XMLNamespaceCount(0),
    XMLCommentCount(0), CDATACount(0),
    processingInstructionCount(0), endDocumentCount(0)
    {}

// get unitCount
int XMLStatsHandler::getUnitCount()
{
    return unitCount;
}

// get loc
int XMLStatsHandler::getLoc()
{
    return loc;
}

// get startDocumentCount
int XMLStatsHandler::getStartDocumentCount()
{
    return startDocumentCount;
}

// get XMLDeclarationCount
int XMLStatsHandler::getXMLDeclarationCount()
{
    return XMLDeclarationCount;
}

// get startTagCount
int XMLStatsHandler::getStartTagCount()
{
    return startTagCount;
}

// get endTagCount
int XMLStatsHandler::getEndTagCount()
{
    return endTagCount;
}

// get charactersCount
int XMLStatsHandler::getCharactersCount()
{
    return charactersCount;
}

// get attributeCount
int XMLStatsHandler::getAttributeCount()
{
    return attributeCount;
}

// get XMLNamespaceCount
int XMLStatsHandler::getXMLNamespaceCount()
{
    return XMLNamespaceCount;
}

// get XMLCommentCount
int XMLStatsHandler::getXMLCommentCount()
{
    return XMLCommentCount;
}

// get CDATACount
int XMLStatsHandler::getCDATACount()
{
    return CDATACount;
}

// get processingInstructionCount
int XMLStatsHandler::getProcessingInstructionCount()
{
    return processingInstructionCount;
}

// get endDocumentCount
int XMLStatsHandler::getEndDocumentCount()
{
    return endDocumentCount;
}

// start Document Handler
void XMLStatsHandler::handleStartDocument() {
    ++startDocumentCount;
}

// XML Declaration Handler
void XMLStatsHandler::handleXMLDeclaration(std::string_view version, std::optional<std::string_view>& encoding, std::optional<std::string_view>& standalone) {
    ++XMLDeclarationCount;
}

// Start Tag Handler
void XMLStatsHandler::handleStartTag(std::string_view qName, std::string_view prefix, std::string_view localName) {
    ++startTagCount;

    if (localName == "unit"sv) {
        ++unitCount;
    }
}

// End Tag Handler
void XMLStatsHandler::handleEndTag(std::string_view qName, std::string_view prefix, std::string_view localName) {
    ++endTagCount;
}

// Character Handler
void XMLStatsHandler::handleCharacter(std::string_view characters) {
    ++charactersCount;

    loc += static_cast<int>(std::count(characters.cbegin(), characters.cend(), '\n'));
}

// attribute Handler
void XMLStatsHandler::handleAttribute(std::string_view qName, std::string_view prefix, std::string_view localName, std::string_view value) {
    ++attributeCount;
}

// XML Namespace Handler
void XMLStatsHandler::handleXMLNamespace(std::string_view prefix, std::string_view uri) {
    ++XMLNamespaceCount;
}

// XML Comment Handler
void XMLStatsHandler::handleXMLComment(std::string_view value) {
    ++XMLCommentCount;
}

// CDATA Handler
void XMLStatsHandler::handleCDATA(std::string_view characters) {
    ++CDATACount;

    loc += static_cast<int>(std::count(characters.cbegin(), characters.cend(), '\n'));
}

// processing Instruction Handler
void XMLStatsHandler::handleProcessingInstruction(std::string_view target, std::string_view data) {
    ++processingInstructionCount;
}

// end Document Handler
void XMLStatsHandler::handleEndDocument() {
    ++endDocumentCount;
}
