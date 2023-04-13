/*
    srcFactsHandler.cpp

    Concrete class specific to srcFacts inheriting from the abstract class XMLParser
*/

#include "srcFactsHandler.hpp"
#include <algorithm>

// provides literal string operator""sv
using namespace std::literals::string_view_literals;

// constructor
srcFactsHandler::srcFactsHandler() :
    textSize(0), loc(0), exprCount(0), functionCount(0),
    classCount(0), unitCount(0), declCount(0), commentCount(0),
    returnCount(0), lineCommentCount(0), stringCount(0)
    {}

// get urls
std::string srcFactsHandler::getUrl()
{
    return url;
}

// get textSizes
int srcFactsHandler::getTextSize()
{
    return textSize;
}

// get locs
int srcFactsHandler::getLoc()
{
    return loc;
}

// get exprCounts
int srcFactsHandler::getExprCount()
{
    return exprCount;
}

// get functionCounts
int srcFactsHandler::getFunctionCount()
{
    return functionCount;
}

// get classCounts
int srcFactsHandler::getClassCount()
{
    return classCount;
}

// get unitCounts
int srcFactsHandler::getUnitCount()
{
    return unitCount;
}

// get declCounts
int srcFactsHandler::getDeclCount()
{
    return declCount;
}

// get commentCounts
int srcFactsHandler::getCommentCount()
{
    return commentCount;
}

// get returnCounts
int srcFactsHandler::getReturnCount()
{
    return returnCount;
}

// get lineCommentCounts
int srcFactsHandler::getLineCommentCount()
{
    return lineCommentCount;
}

// get stringCounts
int srcFactsHandler::getStringCount()
{
    return stringCount;
}

// start Document Handler
void srcFactsHandler::handleStartDocument() {}

// XML Declaration Handler
void srcFactsHandler::handleXMLDeclaration(std::string_view version, std::optional<std::string_view>& encoding, std::optional<std::string_view>& standalone) {}

// Start Tag Handler
void srcFactsHandler::handleStartTag(std::string_view qName, std::string_view prefix, std::string_view localName) {
    if (localName == "expr"sv) {
        ++exprCount;
    } else if (localName == "decl"sv) {
        ++declCount;
    } else if (localName == "comment"sv) {
        ++commentCount;
    } else if (localName == "function"sv) {
        ++functionCount;
    } else if (localName == "unit"sv) {
        ++unitCount;
    } else if (localName == "class"sv) {
        ++classCount;
    } else if (localName == "return"sv) {
        ++returnCount;
    }
}

// End Tag Handler
void srcFactsHandler::handleEndTag(std::string_view qName, std::string_view prefix, std::string_view localName) {}

// Character Handler
void srcFactsHandler::handleCharacter(std::string_view characters) {
    loc += static_cast<int>(std::count(characters.cbegin(), characters.cend(), '\n'));
    textSize += static_cast<int>(characters.size());
}

// attribute Handler
void srcFactsHandler::handleAttribute(std::string_view qName, std::string_view prefix, std::string_view localName, std::string_view value) {
    if (localName == "url"sv) {
        url = value;
    } else if (localName == "type"sv && value == "string"sv) {
        ++stringCount;
    } else if (localName == "type"sv && value == "line") {
        ++lineCommentCount;
    }
}

// XML Namespace Handler
void srcFactsHandler::handleXMLNamespace(std::string_view prefix, std::string_view uri) {}

// XML Comment Handler
void srcFactsHandler::handleXMLComment(std::string_view value) {}

// CDATA Handler
void srcFactsHandler::handleCDATA(std::string_view characters) {
    textSize += static_cast<int>(characters.size());
    loc += static_cast<int>(std::count(characters.cbegin(), characters.cend(), '\n'));
}

// processing Instruction Handler
void srcFactsHandler::handleProcessingInstruction(std::string_view target, std::string_view data) {}

// end Document Handler
void srcFactsHandler::handleEndDocument() {}
