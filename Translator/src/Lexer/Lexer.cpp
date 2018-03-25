//
// Created by igor on 24.03.18.
//

#include "Lexer/Lexer.h"
#include "Lexer/Characters.h"
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


//const std::vector<Token> Lexer::_OPERATORS = {
//        {TokenIdentifier::Equality, "=="},
//        {TokenIdentifier::Addition, "+"},
//        {TokenIdentifier::Multiplication, "*"},
//        {TokenIdentifier::Division, "/"},
//        {TokenIdentifier::Assigment, "="},
//        {TokenIdentifier::Greatness, ">"},
//        {TokenIdentifier::Minority, "<"},
//        {TokenIdentifier::OpenBracket, "("},
//        {TokenIdentifier::CloseBracket, ")"},
//        {TokenIdentifier::OpenCurlyBracker, "}"},
//        {TokenIdentifier::CloseCurlyBracket, "}"},
//        {TokenIdentifier::Comma, ","},
//        {TokenIdentifier::Comment, "//"},
//};



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

char Lexer::getNextNonBlankChar() {
    std::cout << "Skipping spaces: ";
    auto ch = getChar();
    while (Syntax::isBlank(ch)) {
        std::cout << ".";
        ch = getChar();
    }
    std::cout << "\nRead: " << ch << "\n";
    return ch;
}

bool Lexer::eof() const {
    return _file.eof();
}



Token Lexer::nextToken() {
    std::cout << "Reading next token..." << "\n";
    std::string symbol;
    TokenType token_type;
    std::tie(symbol, token_type) = initNewSymbol();

    auto ch = getChar();
    while(!eof()) {
        switch(token_type) {
            case TokenType::Nil:
                if(isBlank(ch)) {
                    ch = getNextNonBlankChar();
                }
                symbol += ch;
                token_type = inferTokenTypeByFirstCharacter(ch);
                break;
            case TokenType::ConstExpr:
                if(isBegginingOfTheIdentifier(ch)) {
                    throw std::runtime_error("No alpha in const number");
                } else if(isPartOfOperator(ch) || isBlank(ch)) {
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
                if(isPartOfOperator(ch) ) {
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
                } else if (isPartOfOperator(ch) || isBlank(ch)) {
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


std::tuple<std::string, TokenType> Lexer::initNewSymbol() const {
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

TokenType Lexer::inferTokenTypeByFirstCharacter(const char ch) const {
    if(isBegginingOfTheIdentifier(ch)) {
        std::cout << "Infering ch as identifier\n";
        return TokenType::Identifier;
    } else if(isDigit(ch)) {
        std::cout << "Infering as Const Expr\n";
        return TokenType::ConstExpr;
    } else if(isPartOfOperator(ch)) {
        std::cout << "Infering ch as an operator\n";
        return TokenType::Operator;
    }
    return TokenType::Nil;
}


