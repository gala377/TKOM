//
// Created by igor on 24.03.18.
//

#include "Lexer/Lexer.h"
#include "Lexer/Characters.h"
#include <iostream>

using namespace Syntax;

// TODO make it so the identifiers and the rest have max lenght

SymbolMap Lexer::_KEYWORDS = {
        {"fn", TokenIdentifier::Function},
        {"loop", TokenIdentifier::Loop},
        {"if", TokenIdentifier::If},
        {"critical", TokenIdentifier::Critical},
        {"concurrent", TokenIdentifier::Concurrent},
        {"let", TokenIdentifier::Variable},
        {"return", TokenIdentifier::Return},
};


SymbolMap Lexer::_OPERATORS = {
        {"==", TokenIdentifier::Equality},
        {"+", TokenIdentifier::Addition},
        {"*", TokenIdentifier::Multiplication},
        {"/", TokenIdentifier::Division},
        {"=", TokenIdentifier::Assigment},
        {">", TokenIdentifier::Greatness},
        {"<", TokenIdentifier::Minority},
        {"(", TokenIdentifier::OpenBracket},
        {")", TokenIdentifier::CloseBracket},
        {"{", TokenIdentifier::OpenCurlyBracker},
        {"}", TokenIdentifier::CloseCurlyBracket},
        {",", TokenIdentifier::Comma},
        {"#", TokenIdentifier::Comment},
};


Lexer::Lexer() {
    _token_assemblers[TokenType::ConstExpr] = std::bind(
            &Lexer::processConstExpr, this, std::placeholders::_1);
    _token_assemblers[TokenType::Identifier] = std::bind(
            &Lexer::processIdentifier, this, std::placeholders::_1);
    _token_assemblers[TokenType::Operator] = std::bind(
            &Lexer::processOperator, this, std::placeholders::_1);
    _token_assemblers[TokenType::Blank] = std::bind(
            &Lexer::processBlankChar, this, std::placeholders::_1);
    _token_assemblers[TokenType::Comment] = std::bind(
            &Lexer::processComment, this, std::placeholders::_1);
}

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
    while(!eof()) {
        auto ch = getNextChar();
        auto token_type = inferTokenTypeByFirstCharacter(ch);

        auto res = _token_assemblers[token_type](ch);

        if(skipToken(res)) {
            continue;
        }

        return res;

    }
    return {TokenIdentifier::Nil, "", _file_line, 0};
}

char Lexer::getNextChar() {
    char ch;
    if(_last_read_ch) {
        ch = _last_read_ch;
        _last_read_ch = '\0';
        std::cout << "Starting from last ch: " << ch << "\n";
    } else {
        ch = getChar();
        std::cout << "Read new char: " << ch << "\n";
    }
    return ch;
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
    } else if(isBlank(ch)) {
        std::cout << "Infering ch as blank\n";
        return TokenType::Blank;
    } else if(isComment(ch)) {
        std::cout << "Infering ch as comment\n";
        return TokenType::Comment;
    }
    // Todo actualy there is a need for a print function
    // to work a little bit different
    // As it takes const char*
    // So make it work or scrap the idea alltogether 
    throw std::runtime_error("Uknown character!");
}


bool Lexer::skipSpaces() const {
    return skip_spaces;
}

bool Lexer::skipNewLines() const {
    return skip_new_lines;
}

bool Lexer::skipBlanks() const {
    return skipSpaces() && skipNewLines();
}

bool Lexer::skipToken(const Token &token) const {
    if(skipSpaces() && token.identifier() == TokenIdentifier::Space) {
        return true;
    } else if (skipNewLines() && token.identifier() == TokenIdentifier::NewLine) {
        return true;
    }
    return false;
}


Token Lexer::processBlankChar(char ch) {
    std::cout << "Char is blank: " << ch << "\n";
    // Could be just in nextToken but this way
    // it saves us some time
    if(skipBlanks()) {
        std::cout << "Skipping blanks\n";
        _last_read_ch = getNextNonBlankChar();
    }
    if(isNewLine(ch)) {
        return newToken(TokenIdentifier::NewLine, {ch});
    }
    return newToken(TokenIdentifier::Space, {ch});
}

Token Lexer::processConstExpr(char ch) {
    auto symbol = assembleConstExpr(ch);
    return newToken(TokenIdentifier::ConstExpr, symbol);
}

Token Lexer::processIdentifier(char ch) {
    auto symbol = assembleIdentifier(ch);
    if(_KEYWORDS.find(symbol) != _KEYWORDS.end()) {
        return newToken((*_KEYWORDS.find(symbol)).second, symbol);
    }
    return newToken(TokenIdentifier::Identifier, symbol);

}

Token Lexer::processComment(char ch) {
    auto symbol = assembleComment(ch);
    return newToken(TokenIdentifier::Comment, symbol);
}

Token Lexer::processOperator(char ch) {
    auto symbol = assembleOperator(ch);
    return newToken((*_OPERATORS.find(symbol)).second, symbol);
}


std::string Lexer::assembleConstExpr(char current) {
    std::cout << "Assembling Const Expr:\n";
    std::string symbol;
    do {
        if (isBegginingOfTheIdentifier(current)) {
            throw std::runtime_error("No alpha in const number");
        } else if (isPartOfOperator(current) || isBlank(current)) {
            _last_read_ch = current;
            return symbol;
        } else if (isDigit(current)) {
            symbol += current;
        } else {
            throw std::runtime_error(
                    "Unexpected char while building a const expr: " + current
            );
        }
        current = getChar();
        std::cout << "Read: " << current << "\n";
        std::cout << "Symbol: " << symbol << "\n";
    } while(!eof());
    if(symbol.empty()) {
        throw std::runtime_error("Could not assemble const expr!");
    }
    return symbol;
}

std::string Lexer::assembleIdentifier(char current) {
    std::cout << "Assembling Identifier:\n";
    std::string symbol;
    do {
        if(isPartOfIdentifier(current)) {
            symbol += current;
        } else if (isPartOfOperator(current) || isBlank(current)) {
            _last_read_ch = current;
            return symbol;
        } else {
            throw std::runtime_error(
                    "Unexpected character while building an identifier: " + current
            );
        }
        current = getChar();
        std::cout << "Read: " << current << "\n";
        std::cout << "Symbol: " << symbol << "\n";
    } while(!eof());
    if(symbol.empty()) {
        throw std::runtime_error("Could not assemble identifier!");
    }
    return symbol;
}

std::string Lexer::assembleOperator(char current) {
    std::cout << "Assembling Operator\n";
    std::string symbol;
    do {
        if (isPartOfOperator(current)) {
            auto guess = symbol + current;
            if (_OPERATORS.find(guess) == _OPERATORS.end()) {
                _last_read_ch = current;
                return symbol;
            }
            symbol = guess;
        } else if (isBlank(current) || isPartOfIdentifier(current) ){
            _last_read_ch = current;
            return symbol;
        } else {
            throw std::runtime_error(
                    "Unexpected char while building an operator: " + current
            );
        }
        current = getChar();
        std::cout << "Read: " << current << "\n";
        std::cout << "Symbol: " << symbol << "\n";
    } while(!eof());
    if (symbol.empty()) {
        throw std::runtime_error("Could not assemble operator");
    }
    return symbol;
}

std::string Lexer::assembleComment(char current) {
    std::cout << "Assembling Comment\n";
    std::string symbol;
    do {
        if(isNewLine(current)) {
            _last_read_ch = current;
            return symbol;
        }
        symbol += current;
        current = getChar(); 
    } while(!eof());
    if(symbol.empty()) {
        throw std::runtime_error("Could not assemble comment");
    }
    return symbol;
}


Token Lexer::newToken(TokenIdentifier id, std::string symbol) const {
    std::cout << "Creating new token: {id: " << int(id) << " sym: " << symbol << "}\n";
    return Token{
            id,
            std::move(symbol),
            _file_line,
            _in_line_position
    };
}