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
        std::fstream _file;
        bool _is_file_opened = false;

        std::uint32_t _file_line = 0;
        std::uint32_t _in_line_position = 0;
        char last_read_ch = '\0';

        char getChar();
        bool eof() const;

        enum class TokenType {
            Nil, Identifier, Keyword, Operator, ConstExpr, Comment
        };

        std::tuple<std::string, TokenType> initEmptySymbol() const;
        TokenType inferTokenTypeByFirstCharacter(const char ch) const;



        bool isNewLine(const char ch) const;
        bool isSpace(const char ch) const;
        bool isBlank(const char ch) const;
        bool isPartOfIdentifier(const char ch) const;
        bool isDigit(const char ch) const;
        bool isAlpha(const char ch) const;
        bool isBegginingOfTheIdentifier(const char ch) const;
        bool isOperator(const char ch) const;

        static shared_seq<Token> _KEYWORDS;
        static shared_seq<Token> _OPERATORS;
    };
}
#endif //TRANSLATOR_LEXER_H