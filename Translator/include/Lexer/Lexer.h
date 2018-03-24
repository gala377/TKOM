//
// Created by igor on 24.03.18.
//

#ifndef TRANSLATOR_LEXER_H
#define TRANSLATOR_LEXER_H

#include <string>
#include <fstream>
#include <vector>

#include "Token.h"

template <typename T>
using shared_seq<T> = static const std::vector<T>;

namespace Syntax {

    class Lexer {
    public:
        void loadFile(std::string path);

    protected:
        std::fstream _file;

        shared_seq<Keyword> _KEYWORDS;
        shared_seq<Operator> _OPERATORS;
    };

}
#endif //TRANSLATOR_LEXER_H