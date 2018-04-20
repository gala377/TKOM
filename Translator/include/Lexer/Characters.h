//
// Created by igor on 25.03.18.
//

#ifndef TRANSLATOR_CHARACTERS_H
#define TRANSLATOR_CHARACTERS_H

#include <set>

namespace Syntax {

    static const std::set<char> _OPERATORS = {
            '=', '/', '\"', '(', ')',
            '{', '}', '+', '-', '*',
            '<', '>', ',',
    };

    bool isNewLine(const char ch);
    bool isSpace(const char ch);
    bool isBlank(const char ch);
    bool isPartOfIdentifier(const char ch);
    bool isDigit(const char ch);
    bool isAlpha(const char ch);
    bool isBegginingOfTheIdentifier(const char ch);
    bool isPartOfOperator(const char ch);
    bool isComment(const char ch);

}


#endif //TRANSLATOR_CHARACTERS_H
