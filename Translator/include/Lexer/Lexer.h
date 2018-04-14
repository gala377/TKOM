//
// Created by igor on 24.03.18.
//

#ifndef TRANSLATOR_LEXER_H
#define TRANSLATOR_LEXER_H

#include <string>
#include <fstream>
#include <vector>
#include <tuple>
#include <map>
#include <functional>

#include "Token.h"

using SymbolMap = const std::map<std::string, Syntax::TokenIdentifier>;

namespace Syntax {

    class Lexer {
    public:
        bool skip_spaces = true;
        bool skip_new_lines = true;

        Lexer();
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
        char _last_read_ch = '\0';

        // Token processing
        std::map<TokenType, std::function<Token(char)>> _token_assemblers;

        bool skipSpaces() const;
        bool skipNewLines() const;
        bool skipBlanks() const;

        bool skipToken(const Token &token) const;

        // Reading methods on file
        // TODO refactor to other class
        char getChar();
        char getNextNonBlankChar();
        bool eof() const;

        // Helpers for reading
        char getNextChar();
        TokenType inferTokenTypeByFirstCharacter(const char ch) const;

        Token processBlankChar(char ch);
        Token processConstExpr(char ch);
        Token processOperator(char ch);
        Token processIdentifier(char ch);
        Token processComment(char ch);

        std::string assembleConstExpr(char current);
        std::string assembleOperator(char current);
        std::string assembleIdentifier(char current);
        std::string assembleComment(char current);

        Token newToken(TokenIdentifier id, std::string symbol) const;

        static SymbolMap _KEYWORDS;
        static SymbolMap _OPERATORS;
    };
}
#endif //TRANSLATOR_LEXER_H