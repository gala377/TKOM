//
// Created by igor on 24.03.18.
//

#include "Lexer/Lexer.h"
#include "Exceptions/Utils.h"
#include <iostream>

using namespace Syntax;

const std::vector<Token> Lexer::_KEYWORDS = {
        {TokenIdentifier::Function, "fn"},
        {TokenIdentifier::Loop, "for"},
        {TokenIdentifier::If, "if"},
        {TokenIdentifier::Critical, "critical"},
        {TokenIdentifier::Concurrent, "concurrent"},
        {TokenIdentifier::Variable, "let"},
};


const std::vector<Token> Lexer::_OPERATORS = {
        {TokenIdentifier::Equality, "=="},
        {TokenIdentifier::Addition, "+"},
        {TokenIdentifier::Multiplication, "*"},
        {TokenIdentifier::Division, "/"},
        {TokenIdentifier::Assigment, "="},
        {TokenIdentifier::Greatness, ">"},
        {TokenIdentifier::Minority, "<"},
        {TokenIdentifier::OpenBracket, "("},
        {TokenIdentifier::CloseBracket, ")"},
        {TokenIdentifier::OpenCurlyBracker, "}"},
        {TokenIdentifier::CloseCurlyBracket, "}"},
        {TokenIdentifier::Comma, ","},
        {TokenIdentifier::Comment, "//"},
};

Lexer::~Lexer() {
    if (_is_file_opened) {
        closeFile();
    }
}

void Lexer::loadFile(std::string path) {
    _file.open(path);
    if(_file.bad()) {
        throw std::runtime_error("Lexer could not open the file");
    }
    _is_file_opened = true;
}

void Lexer::closeFile() {
    _is_file_opened = false;
    _file.close();
}

Token Lexer::nextToken() {
    std::cout << "Reading next token..." << "\n";
    std::string symbol;
    TokenType token_type;
    std::tie(symbol, token_type) = initEmptySymbol();
    // if this is the first line we read
    auto ch = getChar();
    std::cout << "Read: " << ch << "\n";
    if(!last_read_ch) {
        // For now we skip blank characters
        // but that may change later
        std::cout << "Skipping spaces: ";
        while (isBlank(ch)) {
            std::cout << ".";
            ch = getChar();
        }
        std::cout << "\nCurrent char: " << ch << "\n";
    }
    while(!eof()) {
        switch(token_type) {
            case TokenType::Nil:
                symbol += ch;
                token_type = inferTokenTypeByFirstCharacter(ch);
                break;
            case TokenType::ConstExpr:
                if(isBegginingOfTheIdentifier(ch)) {
                    throw std::runtime_error("No alpha in const number");
                } else if(isOperator(ch) || isBlank(ch)) {
                    last_read_ch = ch;
                    return Token{
                            TokenIdentifier::ConstExpr,
                            symbol,
                            _file_line,
                            _in_line_position
                    };
                } else if(isDigit(ch)) {
                    symbol += ch;
                }
                break;
            case TokenType::Operator:
                if( isOperator(ch) ) {
                    symbol += ch;
                } else {
                    last_read_ch = ch;
                    return Token{
                            TokenIdentifier::CloseBracket,
                            symbol,
                            _file_line,
                            _in_line_position
                    };
                }
                break;
            case TokenType::Identifier:
                if(isPartOfIdentifier(ch)) {
                    symbol += ch;
                } else if (isOperator(ch) || isBlank(ch)) {
                    last_read_ch = ch;
                    return Token{
                            TokenIdentifier::Identifier,
                            symbol,
                            _file_line,
                            _in_line_position
                    };
                }
                break;
        }
        ch = getChar();
        std::cout << "Read: " << ch << "\n";
    }
    return {TokenIdentifier::Nil, "", _file_line, 0};
}

std::tuple<std::string, Lexer::TokenType> Lexer::initEmptySymbol() const {
    std::string symbol;
    TokenType token_type = TokenType::Nil;
    if(last_read_ch) {
        if(!isBlank(last_read_ch)) {
            symbol += last_read_ch;
        }
        token_type = inferTokenTypeByFirstCharacter(last_read_ch);
    }
    return std::make_tuple(symbol, token_type);
}

Lexer::TokenType Lexer::inferTokenTypeByFirstCharacter(const char ch) const {
    if(isBegginingOfTheIdentifier(ch)) {
        return TokenType::Identifier;
    } else if(isDigit(ch)) {
        return TokenType::ConstExpr;
    } else if(isOperator(ch)) {
        return TokenType::Operator;
    }
    return TokenType::Nil;
}

char Lexer::getChar() {
    if (!_is_file_opened) {
        throw std::runtime_error("File needs to be opened before reading from it");
    }

    char ch;
    _file >> std::noskipws >> ch;

    if(isNewLine(ch)) {
        _in_line_position = 0;
        ++_file_line;
    } else {
        ++_in_line_position;
    }

    return ch;
}

bool Lexer::eof() const {
    return _file.eof();
}



bool Lexer::isNewLine(const char ch) const {
    return ch == '\n';
}

bool Lexer::isSpace(const char ch) const {
    return isspace(ch);
}

bool Lexer::isBlank(const char ch) const {
    return isblank(ch) | isNewLine(ch);
}

bool Lexer::isPartOfIdentifier(const char ch) const {
    return isBegginingOfTheIdentifier(ch) || isDigit(ch);
}

bool Lexer::isDigit(const char ch) const {
    return isdigit(ch);
}

bool Lexer::isAlpha(const char ch) const {
    return isalpha(ch);
}

bool Lexer::isBegginingOfTheIdentifier(const char ch) const {
    return isAlpha(ch) || ch == '_';
}

bool Lexer::isOperator(const char ch) const {
    for (const auto& op : _OPERATORS) {
        if(op.symbol()[0] == ch) {
            return true;
        }
    }
    return false;
}

