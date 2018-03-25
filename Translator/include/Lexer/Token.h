//
// Created by igor on 24.03.18.
//

#ifndef TRANSLATOR_TOKEN_H
#define TRANSLATOR_TOKEN_H

#include <string>
#include <vector>

namespace Syntax {

    enum class TokenType {
        Nil, Identifier, Keyword, Operator,
        ConstExpr, Comment, Blank,
    };

    enum class TokenIdentifier {

        Nil=0,

        // General identifiers
        Identifier=1, OpenCurlyBracker, CloseCurlyBracket,
        OpenBracket, CloseBracket, Comment,

        ConstExpr=7,

        // Keywords
        Function=8, Loop, If,
        Critical, Concurrent, Variable,

        // Operators
        Equality=14, Addition, Multiplication,
        Division, Assigment, Greatness,
        Minority, Comma,

        //Other
        Space=22,

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
        Token() : _identifier(TokenIdentifier::Nil) {};

        // TODO this needs to be split into 2 different struct
        // One is TOKEN reterned by Lexem
        // Second is map symbol -> identifier
        Token(TokenIdentifier identifier,
              std::string symbol
        ) : _identifier(identifier),
            _symbol(std::move(symbol)),
            _line(0),
            _position(0) {};
        Token(TokenIdentifier identifier,
              std::string symbol,
              std::uint32_t line,
              std::uint32_t position
        ) : _identifier(identifier),
            _symbol(std::move(symbol)),
            _line(line),
            _position(position) {};

        TokenIdentifier identifier() const { return _identifier; }
        std::string symbol() const { return _symbol; }
        std::uint32_t line() const { return _line; }
        std::uint32_t position() const { return _position; };
    protected:
        TokenIdentifier _identifier;
        std::string _symbol;
        std::uint32_t _line;
        std::uint32_t _position;
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
        using Identifier::Token;
    };

    class VariableIdentifier: public Identifier {
    public:
        using Identifier::Token;
    };

}

#endif //TRANSLATOR_TOKEN_H
