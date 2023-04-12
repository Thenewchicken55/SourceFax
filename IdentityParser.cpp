/*
    IdentityParser.cpp

    Concrete class specific to Identity inheriting from the abstract class XMLParser
*/

#include "IdentityParser.hpp"
#include <iostream>
#include <algorithm>

int unclosedBrackets = 0;

// provides literal string operator""sv
using namespace std::literals::string_view_literals;

std::string escape(std::string_view unescaped) {
    std::string unescapedString(unescaped);
    std::string escapedString = "";
    for(auto c: unescapedString) {
        if(c == '<') {
            escapedString += "&lt;";
        } else if (c == '>') {
            escapedString += "&gt;";
        } else if (c == '&') {
            escapedString += "&amp;";
        } else {
            escapedString += c;
        }
    }
    return escapedString;
}

// constructor
IdentityParser::IdentityParser() = default;

//get loc
int IdentityParser::getLoc() {
    return loc;
}

// start Document Handler
void IdentityParser::handleStartDocument() {}

// XML Declaration Handler
void IdentityParser::handleXMLDeclaration(std::string_view version, std::optional<std::string_view>& encoding, std::optional<std::string_view>& standalone) {
    std::cout << "<?xml version=\"" << version << "\" ";

    if (encoding.has_value()) {
        std::cout << "encoding=\"" << encoding.value() << "\" ";
    }

    if (standalone.has_value()) {
        std::cout << "standalone=\"" << standalone.value() << "\"";
    }

    std::cout << "?>" << std::endl;
}

// Start Tag Handler
void IdentityParser::handleStartTag(std::string_view qName, std::string_view prefix, std::string_view localName) {
    if (unclosedBrackets > 0) {
            --unclosedBrackets;
            std::cout << ">";
    }
    std::cout << "<" << qName;
    ++unclosedBrackets;
}

// End Tag Handler
void IdentityParser::handleEndTag(std::string_view qName, std::string_view prefix, std::string_view localName) {
    std::cout << "</" <<  qName << ">";
}

// Character Handler
void IdentityParser::handleCharacter(std::string_view characters) {
    if (unclosedBrackets > 0) {
            --unclosedBrackets;
            std::cout << ">";
    }
    std::cout << escape(characters);

    loc += static_cast<int>(std::count(characters.cbegin(), characters.cend(), '\n'));
}

// attribute Handler
void IdentityParser::handleAttribute(std::string_view qName, std::string_view prefix, std::string_view localName, std::string_view value) {
    std::cout << " " << qName << "=\"" << value << "\"";
}

// XML Namespace Handler
void IdentityParser::handleXMLNamespace(std::string_view prefix, std::string_view uri) {
    std::cout << " xmlns";
    if (!prefix.empty()) {
        std::cout << ":" << prefix;
    }
    std::cout << "=\"" << uri << "\"";
}

// XML Comment Handler
void IdentityParser::handleXMLComment(std::string_view value) {
    std::cout << "<!--" << value << "-->" << std::endl;
}

// CDATA Handler
void IdentityParser::handleCDATA(std::string_view characters) {
    std::cout << "<![CDATA[" << escape(characters) << "]]>";

    loc += static_cast<int>(std::count(characters.cbegin(), characters.cend(), '\n'));
}

// processing Instruction Handler
void IdentityParser::handleProcessingInstruction(std::string_view target, std::string_view data) {
    std::cout << "<?" << target << " " << data << "?>" << std::endl;
}

// end Document Handler
void IdentityParser::handleEndDocument() {}