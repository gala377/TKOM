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

#include "Source.h"
#include "Token.h"

using SymbolMap = const std::map<std::string, Syntax::TokenIdentifier>;

namespace Syntax {

    class Lexer {
    public:
        bool skip_spaces = true;
        bool skip_new_lines = true;

        Lexer(Source& src);
        ~Lexer();

        Token nextToken();
    protected:
        std::map<TokenType, std::function<Token(char)>> _token_assemblers;

        static SymbolMap _KEYWORDS;
        static SymbolMap _OPERATORS;

        Source& _src_file;

        bool skipSpaces() const;
        bool skipNewLines() const;
        bool skipBlanks() const;
        bool skipToken(const Token &token) const;

        TokenType inferTokenTypeByFirstCharacter(char ch) const;

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
    };
}
#endif //TRANSLATOR_LEXER_H