//
// Created by igor on 24.03.18.
//

#include "Lexer/Token.h"

using namespace Syntax;

const std::map<TokenIdentifier, TokenType> Token::_ID_TO_TYPE = {
        {TokenIdentifier::Nil,                  TokenType::Nil},

        {TokenIdentifier::Identifier,           TokenType::Identifier},

        {TokenIdentifier::OpenCurlyBracker,     TokenType::Operator},
        {TokenIdentifier::CloseCurlyBracket,    TokenType::Operator},
        {TokenIdentifier::OpenBracket,          TokenType::Operator},
        {TokenIdentifier::CloseBracket,         TokenType::Operator},
        {TokenIdentifier::Equality,             TokenType::Operator},
        {TokenIdentifier::Addition,             TokenType::Operator},
        {TokenIdentifier::Multiplication,       TokenType::Operator},
        {TokenIdentifier::Division,             TokenType::Operator},
        {TokenIdentifier::Assigment,            TokenType::Operator},
        {TokenIdentifier::Greatness,            TokenType::Operator},
        {TokenIdentifier::Minority,             TokenType::Operator},
        {TokenIdentifier::Comma,                TokenType::Operator},

        {TokenIdentifier::Function,             TokenType::Keyword},
        {TokenIdentifier::Loop,                 TokenType::Keyword},
        {TokenIdentifier::If,                   TokenType::Keyword},
        {TokenIdentifier::Else,                 TokenType::Keyword},
        {TokenIdentifier::Critical,             TokenType::Keyword},
        {TokenIdentifier::Variable,             TokenType::Keyword},
        {TokenIdentifier::Concurrent,           TokenType::Keyword},
        {TokenIdentifier::Return,               TokenType::Keyword},

        {TokenIdentifier::ConstExpr,            TokenType::ConstExpr},

        {TokenIdentifier::Space,                TokenType::Blank},
        {TokenIdentifier::NewLine,              TokenType::Blank},

        {TokenIdentifier::Comment,              TokenType::Comment},
};