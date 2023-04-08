/*
    srcFactsParser.hpp

    Concrete class specific to srcFacts inheriting from the abstract class XMLParser
*/
#ifndef SRC_FACTS_PARSER_HPP
#define SRC_FACTS_PARSER_HPP

#include "XMLParser.hpp"

class srcFactsParser : XMLParser {
public:
    void parse();
};

#endif
