/*
    XMLParser.hpp

    Header file for XML parsing class.
*/

#ifndef XMLPARSER_HPP
#define XMLPARSER_HPP

#include <string_view>
#include <optional>
#include <functional>

class XMLParser {
public:
    // constructor
    XMLParser(std::string_view content);

    // parse XML
    void parse( std::function<void()> startDocumentHandler,
                std::function<void(std::string_view& version, std::optional<std::string_view>& encoding, std::optional<std::string_view>& standalone)> XMLDeclarationHandler,
                std::function<void(std::string_view& qName, std::string_view& prefix, std::string_view& localName)> elementStartTagHandler, 
                std::function<void(std::string_view& qName, std::string_view& prefix, std::string_view& localName)> elementEndTagHandler, 
                std::function<void(std::string_view& characters)> characterEntityReferencesHandler,
                std::function<void(std::string_view& characters)> characterNonEntityReferencesHandler,
                int& textSize, int& loc, std::string& url, std::function<void(std::string_view localName, std::string_view value)> incrementAttributesHandler);
    
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
    void parseCharacterEntityReference();

    // parse character non-entity references
    void parseCharacterNotEntityReference(std::string_view& characters);

    // parse XML comment
    void parseComment(bool& doneReading);

    // parse CDATA
    void parseCDATA(bool& doneReading, std::string_view& characters);

    // parse processing instruction
    void parseProcessing();

    // parse end tag
    void parseEndTag();

    // parse start tag
    void parseStartTag(std::string_view& prefix, std::string_view& qName, std::string_view& localName);

    // parse XML namespace
    void parseNamespace();

    // parse attribute
    std::string_view parseAttribute();

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

    // wrapper for remove_prefix()
    void removePrefix(int indexToRemove);

    // wrapper for find_first_not_of()
    std::size_t findFirstNotOf(std::string_view delimiters);

    // wrapper for size()
    int sizeOfContent();

    // wrapper for .npos
    size_t npos();

    // wrapper for compare()
    int compareContent(int startingPosition, int lengthOfString, const std::string& compareToString);

    // uses xmlNameMask bitset to compare with the first char of content
    bool isMatchNameMask();

    // data members
    std::string_view content;

    long totalBytes;

    std::string_view unescapedCharacter;
    std::string_view escapedCharacter;
};
#endif
