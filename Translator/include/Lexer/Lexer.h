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

using SymbolMap = const std::map<std::string, Syntax::Token::Identifier>;

namespace Syntax {

    class Lexer {
    public:
        bool skip_spaces = true;
        bool skip_new_lines = true;

        Lexer(Source& src);
        ~Lexer();

        Token::Token nextToken();
    protected:
        std::map<Token::Type, std::function<Token::Token(char)>> _token_assemblers;

        static SymbolMap _KEYWORDS;
        static SymbolMap _OPERATORS;

        Source& _src_file;

        bool skipSpaces() const;
        bool skipNewLines() const;
        bool skipBlanks() const;
        bool skipToken(const Token::Token &token) const;

        Token::Type inferTokenTypeByFirstCharacter(char ch) const;

        Token::Token processBlankChar(char ch);
        Token::Token processConstExpr(char ch);
        Token::Token processOperator(char ch);
        Token::Token processIdentifier(char ch);
        Token::Token processComment(char ch);

        std::string assembleConstExpr(char current);
        std::string assembleOperator(char current);
        std::string assembleIdentifier(char current);
        std::string assembleComment(char current);

        Token::Token newToken(Token::Identifier id, std::string symbol) const;

        template <class Exception, class ...Args>
        Exception exception(Args... args) const {
            return Exception(_src_file.line()+1, _src_file.inLinePosition(), args...);
        };
    };
}
#endif //TRANSLATOR_LEXER_H