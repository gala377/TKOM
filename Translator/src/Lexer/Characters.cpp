//
// Created by igor on 25.03.18.
//

#include "Lexer/Characters.h"

#include <string>
#include <iostream>

using namespace Syntax;


bool Syntax::isNewLine(const char ch)  {
    return ch == '\n';
}

bool Syntax::isSpace(const char ch)  {
    return isspace(ch);
}

bool Syntax::isBlank(const char ch)  {
    return isblank(ch) | Syntax::isNewLine(ch);
}

bool Syntax::isPartOfIdentifier(const char ch)  {
    return isBegginingOfTheIdentifier(ch) || isDigit(ch);
}

bool Syntax::isDigit(const char ch)  {
    return isdigit(ch);
}

bool Syntax::isAlpha(const char ch)  {
    return isalpha(ch);
}

bool Syntax::isBegginingOfTheIdentifier(const char ch)  {
    return Syntax::isAlpha(ch) || ch == '_';
}

bool Syntax::isPartOfOperator(const char ch) {
    return (bool)_OPERATORS.count(ch);
}