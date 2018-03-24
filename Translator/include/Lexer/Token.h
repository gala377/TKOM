//
// Created by igor on 24.03.18.
//

#ifndef TRANSLATOR_TOKEN_H
#define TRANSLATOR_TOKEN_H

#include <string>
#include <vector>

namespace Syntax {

    /**
     * Token represents elements returned by getNextToken method
     * provided by Lexem.
     *
     * identifier is a name that a token is know under
     * symbol is a character sequence representing token in code
     */
    class Token {
    public:
        Token() = default;
        Token(std::string identifier, std::string symbol) : _identifier(std::move(identifier)), _symbol(std::move(symbol)) {};

        std::string identifier() { return _identifier; }
        std::string symbol() { return _symbol; }
    protected:
        std::string _identifier;
        std::string _symbol;
    };

    class Keyword : public Token {
    public:
        Keyword() = default;
        Keyword(std::string identifier, std::string symbol) : Token(std::move(identifier), std::move(symbol)) {};
    };

    class Operator: public Keyword {

    };

    /**
     * Identifier is special case token it's symbol is derived from
     * the file parsed
     */
    class Identifier: public Token {
    };

    class FunctionIdentifier: public Identifier {
    };

    class VariableIdentifier: public Identifier {
    public:
        std::int64_t value;
    };

    const std::vector<Keyword> KEYWORDS = {
            {"Function", "fn"},
            {"If", "if"},
            {"For", "for"},
            {"Variable", "let"},
            {"Critical", "critical"},
            {"Concurrent", "concurrent"},
    };

    const std::vector<Token> OPERATORS = {
            {"Plus", "+"},
            {"Minus", "-"},
            {"Multiplication", "*"},
            {"Derivation", "/"},
            {"Equality", "=="},
            {"Comparision_GE"}

    };

}

#endif //TRANSLATOR_TOKEN_H
