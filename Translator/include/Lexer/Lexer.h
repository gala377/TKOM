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
#include <stack>

#include "Source.h"
#include "Token.h"


// TODO Lexer bug - freezes if new lines at the end of the file

namespace Syntax {

    using AssemblersMap = std::map<Token::Type, std::function<Token::Token(char)>>;
    using SymbolMap = const std::map<std::string, Token::Identifier>;
    using Context = std::stack<std::pair<bool, bool>>;

    class Lexer {
    public:
        bool skip_spaces = true;
        bool skip_new_lines = true;

        Lexer(Source& src);
        ~Lexer();

        Token::Token nextToken();
        void ungetToken(Token::Token token);

        std::pair<bool, bool> newContext(bool spaces, bool new_lines);
        void retrieveContext();

        std::tuple<int, int> inFilePosition() const;
    protected:
        std::stack<Token::Token> _pending_tokens;

        AssemblersMap _token_assemblers;
        Context _context;

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
        Token::Token processString(char ch);

        std::string assembleConstExpr(char current);
        std::string assembleOperator(char current);
        std::string assembleIdentifier(char current);
        std::string assembleComment(char current);
        std::string assembleString(char current);

        Token::Token newToken(Token::Identifier id, std::string symbol) const;

        template <class Exception, class ...Args>
        Exception exception(Args... args) const {
            return Exception(_src_file.line()+1, _src_file.inLinePosition(), args...);
        };
    };
}
#endif //TRANSLATOR_LEXER_H