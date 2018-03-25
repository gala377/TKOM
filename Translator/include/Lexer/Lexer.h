//
// Created by igor on 24.03.18.
//

#ifndef TRANSLATOR_LEXER_H
#define TRANSLATOR_LEXER_H

#include <string>
#include <fstream>
#include <vector>
#include <tuple>

#include "Token.h"

template <typename T>
using shared_seq = const std::vector<T>;

namespace Syntax {

    class Lexer {
    public:
        ~Lexer();

        void loadFile(std::string path);
        void closeFile();

        Token nextToken();
    protected:
        // File information
        std::fstream _file;
        bool _is_file_opened = false;

        // Reading information
        std::uint32_t _file_line = 0;
        std::uint32_t _in_line_position = 0;
        char last_read_ch = '\0';

        // Reading methods
        char getChar();
        char getNextNonBlankChar();
        bool eof() const;

        // Helpers for reading
        std::tuple<std::string, TokenType> initNewSymbol() const;
        TokenType inferTokenTypeByFirstCharacter(const char ch) const;


        static shared_seq<Token> _KEYWORDS;
//        static shared_seq<Token> _OPERATORS;
    };
}
#endif //TRANSLATOR_LEXER_H