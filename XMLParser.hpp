#ifndef XMLPARSER_HPP
#define XMLPARSER_HPP

#include <string_view>
#include <utility>
#include <string>

class XMLParser {
public:
    // constructor
    XMLParser(std::string_view content);

    // get doneReading
    bool isDoneReading();

    // set doneReading
    // @param[in] change doneReading to newValue
    void setDoneReading(bool newValue);

    // get qName
    std::string_view getQName();

    // set qName
    void setQName(std::string_view newQName);

    // parse XML declaration
    void parseXMLDeclaration();

    // parse required version
    void parseVersion();

    // parse optional encoding and standalone attributes
    void parseEncodingAndStandalone();

    //parse DOCTYPE
    void parseDOCTYPE();

    // refill content preserving unprocessed
    int refillPreserve();

    // parse character entity references
    void parseCharacterEntityReference();

    // parse character non-entity references
    std::string_view parseCharacterNotEntityReference();

    // parse XML comment
    int parseComment();

    // parse CDATA
    std::pair<int, std::string_view> parseCDATA();

    // parse processing instruction
    void parseProcessing();

    // parse end tag
    void parseEndTag();

    // parse start tag
    std::size_t parseStartTag(std::string_view& localName);

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
    int parseBegin();

    // Accessor::predicate to check if content has a specific character at a specific index
    bool isCharacter(int index, char character);

    // wrapper for remove_prefix()
    void removePrefix(int indexToRemove);

    // wrapper for find_first_not_of()
    std::size_t findFirstNotOf(std::string_view delimiters);
    
    // wrapper for size()
    int getSizeOfContent();

    // wrapper for .npos
    size_t npos();

    // @return content[0]
    char getFirstCharFromContent();
    
    // wrapper for compare()
    int compareContent(int startingPosition, int lengthOfString, std::string compareToString);

private:
    std::string_view content;
    bool doneReading = false;
    std::string_view qName;
    std::string_view prefix;

};
#endif
