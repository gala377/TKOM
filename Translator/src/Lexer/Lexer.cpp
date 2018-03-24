//
// Created by igor on 24.03.18.
//

#include "Lexer/Lexer.h"
#include "Exceptions/Utils.h"
#include <iostream>

using namespace Syntax;

// todo decide on token representation structure
const std::vector<Keyword> Lexer::_KEYWORDS = {

};


const std::vector<Operator> Lexer::_OPERATORS = {

};

void Lexer::loadFile(std::string path) {
    throw UnimplementedError();
}