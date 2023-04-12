/*
    srcFactsParser.cpp

    Concrete class specific to srcFacts inheriting from the abstract class XMLParser
*/

#include "srcFactsParser.hpp"
#include <algorithm>

// provides literal string operator""sv
using namespace std::literals::string_view_literals;

// constructor
srcFactsParser::srcFactsParser() :
    textSize(0), loc(0), exprCount(0), functionCount(0),
    classCount(0), unitCount(0), declCount(0), commentCount(0),
    returnCount(0), lineCommentCount(0), stringCount(0)
    {}

// get urls
std::string srcFactsParser::getUrl()
{
    return url;
}

// get textSizes
int srcFactsParser::getTextSize()
{
    return textSize;
}

// get locs
int srcFactsParser::getLoc()
{
    return loc;
}

// get exprCounts
int srcFactsParser::getExprCount()
{
    return exprCount;
}

// get functionCounts
int srcFactsParser::getFunctionCount()
{
    return functionCount;
}

// get classCounts
int srcFactsParser::getClassCount()
{
    return classCount;
}

// get unitCounts
int srcFactsParser::getUnitCount()
{
    return unitCount;
}

// get declCounts
int srcFactsParser::getDeclCount()
{
    return declCount;
}

// get commentCounts
int srcFactsParser::getCommentCount()
{
    return commentCount;
}

// get returnCounts
int srcFactsParser::getReturnCount()
{
    return returnCount;
}

// get lineCommentCounts
int srcFactsParser::getLineCommentCount()
{
    return lineCommentCount;
}

// get stringCounts
int srcFactsParser::getStringCount()
{
    return stringCount;
}

// start Document Handler
void srcFactsParser::handleStartDocument() {}

// XML Declaration Handler
void srcFactsParser::handleXMLDeclaration(std::string_view version, std::optional<std::string_view>& encoding, std::optional<std::string_view>& standalone) {}

// Start Tag Handler
void srcFactsParser::handleStartTag(std::string_view qName, std::string_view prefix, std::string_view localName) {
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
void srcFactsParser::handleEndTag(std::string_view qName, std::string_view prefix, std::string_view localName) {}

// Character Handler
void srcFactsParser::handleCharacter(std::string_view characters) {
    loc += static_cast<int>(std::count(characters.cbegin(), characters.cend(), '\n'));
    textSize += static_cast<int>(characters.size());
}

// attribute Handler
void srcFactsParser::handleAttribute(std::string_view qName, std::string_view prefix, std::string_view localName, std::string_view value) {
    if (localName == "url"sv) {
        url = value;
    } else if (localName == "type"sv && value == "string"sv) {
        ++stringCount;
    } else if (localName == "type"sv && value == "line") {
        ++lineCommentCount;
    }
}

// XML Namespace Handler
void srcFactsParser::handleXMLNamespace(std::string_view prefix, std::string_view uri) {}

// XML Comment Handler
void srcFactsParser::handleXMLComment(std::string_view value) {}

// CDATA Handler
void srcFactsParser::handleCDATA(std::string_view characters) {
    textSize += static_cast<int>(characters.size());
    loc += static_cast<int>(std::count(characters.cbegin(), characters.cend(), '\n'));
}

// processing Instruction Handler
void srcFactsParser::handleProcessingInstruction(std::string_view target, std::string_view data) {}

// end Document Handler
void srcFactsParser::handleEndDocument() {}
