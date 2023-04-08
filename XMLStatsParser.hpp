/*
    XMLStatsParser.hpp

    Header file for the concrete class specific to XML Stats inheriting from the abstract class XMLParser
*/

#ifndef SRC_FACTS_PARSER_HPP
#define SRC_FACTS_PARSER_HPP

#include "XMLStatsParser.hpp"
#include "XMLParser.hpp"
#include <string>

// provides literal string operator""sv
using namespace std::literals::string_view_literals;

class XMLStatsParser : public XMLParser {
public:
    // constructor
    XMLStatsParser(std::string_view content);

    // get url
    std::string getUrl();

    // get unitCount
    int getUnitCount();

    // get loc
    int getLoc();

    // get startDocumentCount
    int getStartDocumentCount();

    // get XMLDeclarationCount
    int getXMLDeclarationCount();

    // get startTagCount
    int getStartTagCount();

    // get endTagCount
    int getEndTagCount();

    // get charactersCount
    int getCharactersCount();

    // get attributeCount
    int getAttributeCount();

    // get XMLNamespaceCount
    int getXMLNamespaceCount();

    // get XMLCommentCount
    int getXMLCommentCount();

    // get CDATACount
    int getCDATACount();

    // get processingInstructionCount
    int getProcessingInstructionCount();

    // get endDocumentCount
    int getEndDocumentCount();

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
    int unitCount;
    int loc;
    int startDocumentCount;
    int XMLDeclarationCount;
    int startTagCount;
    int endTagCount;
    int charactersCount;
    int attributeCount;
    int XMLNamespaceCount;
    int XMLCommentCount;
    int CDATACount;
    int processingInstructionCount;
    int endDocumentCount;
};

#endif
