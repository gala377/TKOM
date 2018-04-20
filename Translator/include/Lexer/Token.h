//
// Created by igor on 24.03.18.
//

#ifndef TRANSLATOR_TOKEN_H
#define TRANSLATOR_TOKEN_H

#include <string>
#include <vector>
#include <map>

namespace Syntax::Token {

    enum class Type {
        Nil, Identifier, Keyword, Operator,
        ConstExpr, Comment, Blank,
    };

    enum class Identifier {

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

    class Token {
    public:
        Token();
        Token(Identifier identifier, std::string symbol);
        Token(Identifier identifier,
              std::string symbol,
              std::uint32_t line,
              std::uint32_t position);

        Identifier identifier() const;
        std::string symbol() const;
        std::uint32_t line() const;
        std::uint32_t position() const;
        Type type() const;
    protected:
        Identifier _identifier;
        Type _type;
        std::string _symbol;
        std::uint32_t _line;
        std::uint32_t _position;

        static const std::map<Identifier, Type> _ID_TO_TYPE;
    };

}

#endif //TRANSLATOR_TOKEN_H
