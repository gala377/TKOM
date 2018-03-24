//
// Created by igor on 24.03.18.
//

#ifndef TRANSLATOR_TOKEN_H
#define TRANSLATOR_TOKEN_H

#include <string>
#include <vector>

namespace Syntax {

    enum class TokenIdentifier {};

    // todo decide on structure
    enum class Keywords : public TokenIdentifier {
        FUNCTION,
        LOOP,
        CRITICAL_SECTION,
        IF,
        CONCURRENT_BLOCK,

    };

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
        Token(TokenIdentifier identifier, std::string symbol) : _identifier(identifier), _symbol(std::move(symbol)) {};

        TokenIdentifier identifier() { return _identifier; }
        std::string symbol() { return _symbol; }
    protected:
        TokenIdentifier _identifier;
        std::string _symbol;
    };

    /**
     * Keyword represents language specific character sequence
     * with special meaning for execution
     */
    class Keyword : public Token {
    public:
        using Token::Token;
    };

    /**
     * Operator is a keyword with regards to it not being representet
     * By letters or numbers
     */
    class Operator: public Keyword {
        using Keyword::Token;
    };

    /**
     * Identifier is special case token. Tt's symbol is derived from
     * the file parsed
     */
    class Identifier: public Token {
    public:
        using Token::Token;
    };

    class FunctionIdentifier: public Identifier {
    public:
        using Identifier::Token
    };

    class VariableIdentifier: public Identifier {
    public:
        using Identifier::Token;
    };

}

#endif //TRANSLATOR_TOKEN_H
