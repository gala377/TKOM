//
// Created by igor on 24.03.18.
//

#include "Lexer/Token.h"

using namespace Syntax::Token;

const std::map<Identifier, Type> Token::_ID_TO_TYPE = {
        {Identifier::Nil,                  Type::Nil},

        {Identifier::Identifier,           Type::Identifier},

        {Identifier::OpenCurlyBracker,     Type::Operator},
        {Identifier::CloseCurlyBracket,    Type::Operator},
        {Identifier::OpenBracket,          Type::Operator},
        {Identifier::CloseBracket,         Type::Operator},
        {Identifier::Equality,             Type::Operator},
        {Identifier::Addition,             Type::Operator},
        {Identifier::Multiplication,       Type::Operator},
        {Identifier::Division,             Type::Operator},
        {Identifier::Assigment,            Type::Operator},
        {Identifier::Greatness,            Type::Operator},
        {Identifier::Minority,             Type::Operator},
        {Identifier::Comma,                Type::Operator},

        {Identifier::Function,             Type::Keyword},
        {Identifier::Loop,                 Type::Keyword},
        {Identifier::If,                   Type::Keyword},
        {Identifier::Else,                 Type::Keyword},
        {Identifier::Critical,             Type::Keyword},
        {Identifier::Variable,             Type::Keyword},
        {Identifier::Concurrent,           Type::Keyword},
        {Identifier::Return,               Type::Keyword},

        {Identifier::ConstExpr,            Type::ConstExpr},

        {Identifier::Space,                Type::Blank},
        {Identifier::NewLine,              Type::Blank},

        {Identifier::Comment,              Type::Comment},
};


Token::Token() : _identifier(Identifier::Nil),
                 _type(Type::Nil),
                 _line(0),
                 _position(0) {};

Token::Token(Identifier identifier, std::string symbol) : _identifier(identifier),
                                                          _type((*_ID_TO_TYPE.find(identifier)).second),
                                                          _symbol(std::move(symbol)),
                                                          _line(0),
                                                          _position(0) {};

Token::Token(Identifier identifier,
             std::string symbol,
             std::uint32_t line,
             std::uint32_t position) : _identifier(identifier),
                                       _type((*_ID_TO_TYPE.find(identifier)).second),
                                       _symbol(std::move(symbol)),
                                       _line(line),
                                       _position(position) {};



Identifier Token::Token::identifier() const {
    return _identifier;
}

std::string Token::Token::symbol() const {
    return _symbol;
}

std::uint32_t Token::Token::line() const {
    return _line;
}

std::uint32_t Token::Token::position() const {
    return _position;
};

Type Token::Token::type() const {
    return _type;
}