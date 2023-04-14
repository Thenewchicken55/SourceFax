/*
    XMLParserHandler.hpp

    header file for an interface that handles the user-defined processing
    and handlers(what we previously had as callbacks).

*/

#ifndef XMLPARSERHANDLER_HPP
#define XMLPARSERHANDLER_HPP

#include <string_view>
#include <optional>


class XMLParserHandler {
public:

    // start Document Handler
    virtual void handleStartDocument() {};

    // XML Declaration Handler
    virtual void handleXMLDeclaration(std::string_view version, std::optional<std::string_view>& encoding, std::optional<std::string_view>& standalone) {};

    // start Tag Handler
    virtual void handleStartTag(std::string_view qName, std::string_view prefix, std::string_view localName) {};

    // end Tag Handler
    virtual void handleEndTag(std::string_view qName, std::string_view prefix, std::string_view localName) {};

    // character Handler
    virtual void handleCharacter(std::string_view characters) {};

    // attribute Handler
    virtual void handleAttribute(std::string_view qName, std::string_view prefix, std::string_view localName, std::string_view value) {};

    // XML Namespace Handler
    virtual void handleXMLNamespace(std::string_view prefix, std::string_view uri) {};

    // XML Comment Handler
    virtual void handleXMLComment(std::string_view value) {};

    // CDATA Handler
    virtual void handleCDATA(std::string_view characters) {};

    // processing Instruction Handler
    virtual void handleProcessingInstruction(std::string_view target, std::string_view data) {};

    // end Document Handler
    virtual void handleEndDocument() {};

};

#endif
