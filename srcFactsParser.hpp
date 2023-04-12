/*
    srcFactsParser.hpp

    Concrete class specific to srcFacts inheriting from the abstract class XMLParser
*/
#ifndef SRCFACTSPARSER_HPP
#define SRCFACTSPARSER_HPP

#include <string>
#include "XMLParserHandler.hpp"

class srcFactsParser : public XMLParserHandler {
public:
    // constructor
    srcFactsParser();

    // get url
    std::string getUrl();

    // get textSize
    int getTextSize();

    // get loc
    int getLoc();

    // get exprCount
    int getExprCount();

    // get functionCount
    int getFunctionCount();

    // get classCount
    int getClassCount();

    // get unitCount
    int getUnitCount();

    // get declCount
    int getDeclCount();

    // get commentCount
    int getCommentCount();

    // get returnCount
    int getReturnCount();

    // get lineCommentCount
    int getLineCommentCount();

    // get stringCount
    int getStringCount();

protected:
    // start Document Handler
    void handleStartDocument() override;

    // XML Declaration Handler
    void handleXMLDeclaration(std::string_view version, std::optional<std::string_view>& encoding, std::optional<std::string_view>& standalone) override;

    // Start Tag Handler
    void handleStartTag(std::string_view qName, std::string_view prefix, std::string_view localName) override;

    // End Tag Handler
    void handleEndTag(std::string_view qName, std::string_view prefix, std::string_view localName) override;

    // Character Handler
    void handleCharacter(std::string_view characters) override;

    // attribute Handler
    void handleAttribute(std::string_view qName, std::string_view prefix, std::string_view localName, std::string_view value) override;

    // XML Namespace Handler
    void handleXMLNamespace(std::string_view prefix, std::string_view uri) override;

    // XML Comment Handler
    void handleXMLComment(std::string_view value) override;

    // CDATA Handler
    void handleCDATA(std::string_view characters) override;

    // processing Instruction Handler
    void handleProcessingInstruction(std::string_view target, std::string_view data) override;

    // end Document Handler
    void handleEndDocument() override;

private:
    std::string url;
    int textSize;
    int loc;
    int exprCount;
    int functionCount;
    int classCount;
    int unitCount;
    int declCount;
    int commentCount;
    int returnCount;
    int lineCommentCount;
    int stringCount;
};

#endif
