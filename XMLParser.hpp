/*
    XMLParser.hpp

    Header file for XML parsing class.
*/

#ifndef XMLPARSER_HPP
#define XMLPARSER_HPP

#include "XMLParserHandler.hpp"
#include <string_view>
#include <optional>
#include <functional>

class XMLParser {
public:
    // constructor
    XMLParser(std::string_view content, XMLParserHandler& handler);

    // parse XML
    void parse();

    // get totalBytes
    long getTotalBytes();

private:
    // parse XML declaration
    void parseXMLDeclaration(std::string_view& version, std::optional<std::string_view>& encoding, std::optional<std::string_view>& standalone);

    // parse required version
    void parseVersion(std::string_view& version);

    // parse optional encoding attribute
    void parseEncoding(std::optional<std::string_view>& encoding);

    // parse optional standalone attribute
    void parseStandalone(std::optional<std::string_view>& standalone);

    //parse DOCTYPE
    void parseDOCTYPE();

    // refill content preserving unprocessed
    void refillPreserve(bool& doneReading);

    // parse character entity references
    std::string_view parseCharacterEntityReference();

    // parse character non-entity references
    std::string_view parseCharacterNotEntityReference();

    // parse XML comment
    std::string_view parseComment(bool& doneReading);

    // parse CDATA
    void parseCDATA(bool& doneReading, std::string_view& characters);

    // parse processing instruction
    std::pair<std::string_view, std::string_view> parseProcessing();

    // parse end tag
    void parseEndTag(std::string_view& qName, std::string_view& prefix, std::string_view& localName);

    // parse start tag
    void parseStartTag(std::string_view& qName, std::string_view& prefix, std::string_view& localName);

    // parse XML namespace
    std::pair<std::string_view, std::string_view> parseNamespace();

    // parse attribute
    std::string_view parseAttribute(std::string_view& qName, [[maybe_unused]] std::string_view& prefix, std::string_view& localName);

    // Accessor::predicate to test if the tag is a XML declaration
    bool isXML();

    // Accessor::predicate to test if the tag is DOCTYPE
    bool isDOCTYPE();

    // Accessor::predicate to test if the tag is CDATA
    bool isCDATA();

    // Accessor::predicate to test if the tag is a comment tag
    bool isComment();

    // Accessor::predicate to test if the tag is an XML namespace
    bool isNamespace();

    // parse file from the start
    void parseBegin();

    // Accessor::predicate to check if content has a specific character at a specific index
    bool isCharacter(int index, char character);

    // data members
    std::string_view content;

    long totalBytes;

    XMLParserHandler& handler;
};
#endif
