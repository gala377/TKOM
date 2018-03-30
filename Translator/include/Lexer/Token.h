//
// Created by igor on 24.03.18.
//

#ifndef TRANSLATOR_TOKEN_H
#define TRANSLATOR_TOKEN_H

#include <string>
#include <vector>
#include <map>

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
        Function=8, Loop, If, Else,
        Critical, Concurrent, Variable,
        Return,

        // Operators
        Equality=16, Addition, Multiplication,
        Division, Assigment, Greatness,
        Minority, Comma,

        //Other
        Space=24, NewLine,

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
        Token() : _identifier(TokenIdentifier::Nil),
                  _type(TokenType::Nil),
                  _line(0),
                  _position(0) {};

        Token(TokenIdentifier identifier, std::string symbol) : _identifier(identifier),
                                                                _type((*_ID_TO_TYPE.find(identifier)).second),
                                                                _symbol(std::move(symbol)),
                                                                _line(0),
                                                                _position(0) {};
        Token(TokenIdentifier identifier,
              std::string symbol,
              std::uint32_t line,
              std::uint32_t position) : _identifier(identifier),
                                        _type((*_ID_TO_TYPE.find(identifier)).second),
                                        _symbol(std::move(symbol)),
                                        _line(line),
                                        _position(position) {};




        TokenIdentifier identifier() const { return _identifier; }
        std::string symbol() const { return _symbol; }
        std::uint32_t line() const { return _line; }
        std::uint32_t position() const { return _position; };
        TokenType type() const { return _type; }
    protected:
        TokenIdentifier _identifier;
        TokenType _type;
        std::string _symbol;
        std::uint32_t _line;
        std::uint32_t _position;

        static const std::map<TokenIdentifier, TokenType> _ID_TO_TYPE;
    };

}

#endif //TRANSLATOR_TOKEN_H
