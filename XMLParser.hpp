#ifndef XMLPARSER_HPP
#define XMLPARSER_HPP

#include <string_view>
#include <utility>

class XMLParser {
public:
    // constructor
    XMLParser();

    // parse XML declaration
    void parseXMLDeclaration(std::string_view content);

    // parse required version
    void parseVersion(std::string_view& content);

    // parse optional encoding and standalone attributes
    void parseEncodingAndStandalone(std::string_view& content);

    //parse DOCTYPE
    void parseDOCTYPE(std::string_view& content);

    // refill content preserving unprocessed
    int refillPreserve(std::string_view& content, bool& doneReading);

    // parse character entity references
    void parseCharacterEntityReference(std::string_view& content);

    // parse character non-entity references
    std::string_view parseCharacterNotEntityReference(std::string_view& content);

    // parse XML comment
    int parseComment(std::string_view& content, bool& doneReading);

    // parse CDATA
    std::pair<int, std::string_view> parseCDATA(std::string_view& content, bool& doneReading);

    // parse processing instruction
    void parseProcessing(std::string_view& content);

    // parse end tag
    void parseEndTag(std::string_view& content);

    // parse start tag
    std::size_t parseStartTag(std::string_view& content, std::string_view& qName, std::string_view& prefix, std::string_view& localName);

    // parse XML namespace
    void parseNamespace(std::string_view& content);

    // parse attribute
    std::size_t parseAttribute(std::string_view& content);

    // Accessor::predicate to test if the tag is a XML declaration
    bool isXML(std::string_view content);

    // Accessor::predicate to test if the tag is DOCTYPE
    bool isDOCTYPE(std::string_view content);

    // Accessor::predicate to test if the tag is CDATA
    bool isCDATA(std::string_view content);

    // Accessor::predicate to test if the tag is a comment tag
    bool isComment(std::string_view content);

    // Accessor::predicate to test if the tag is an XML namespace
    bool isNamespace(std::string_view content);

    // parse file from the start
    int parseBegin(std::string_view& content);

    // Accessor::predicate to check if content has a specific character at a specific index
    bool isCharacter(std::string_view& content, int index, char character);

};
#endif
