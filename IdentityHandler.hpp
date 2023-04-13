/*
    IdentityHandler.hpp

    Concrete class specific to Identity inheriting from the abstract class XMLParser
*/

#ifndef IDENTITYHANDLER_HPP
#define IDENTITYHANDLER_HPP

#include "XMLParserHandler.hpp"
#include <string>

class IdentityHandler : public XMLParserHandler {
public:
    // constructor
    IdentityHandler();

    //get loc
    int getLoc();

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
    int loc = 0;
};

#endif
