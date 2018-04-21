//
// Created by igor on 24.03.18.
//

#include <iostream>

#include "Lexer/Lexer.h"
#include "Lexer/Characters.h"
#include "Lexer/Exceptions.h"

using namespace Syntax;

// TODO make it so the identifiers and the rest have max lenght

SymbolMap Lexer::_KEYWORDS = {
        {"fn", Token::Identifier::Function},
        {"loop", Token::Identifier::Loop},
        {"if", Token::Identifier::If},
        {"critical", Token::Identifier::Critical},
        {"concurrent", Token::Identifier::Concurrent},
        {"let", Token::Identifier::Variable},
        {"return", Token::Identifier::Return},
};


SymbolMap Lexer::_OPERATORS = {
        {"==", Token::Identifier::Equality},
        {"+", Token::Identifier::Addition},
        {"*", Token::Identifier::Multiplication},
        {"/", Token::Identifier::Division},
        {"=", Token::Identifier::Assigment},
        {">", Token::Identifier::Greatness},
        {"<", Token::Identifier::Minority},
        {"(", Token::Identifier::OpenBracket},
        {")", Token::Identifier::CloseBracket},
        {"{", Token::Identifier::OpenCurlyBracker},
        {"}", Token::Identifier::CloseCurlyBracket},
        {",", Token::Identifier::Comma},
        {"#", Token::Identifier::Comment},
};


Lexer::Lexer(Source& src): _src_file(src) {
    _token_assemblers[Token::Type::ConstExpr] = std::bind(
            &Lexer::processConstExpr, this, std::placeholders::_1);
    _token_assemblers[Token::Type::Identifier] = std::bind(
            &Lexer::processIdentifier, this, std::placeholders::_1);
    _token_assemblers[Token::Type::Operator] = std::bind(
            &Lexer::processOperator, this, std::placeholders::_1);
    _token_assemblers[Token::Type::Blank] = std::bind(
            &Lexer::processBlankChar, this, std::placeholders::_1);
    _token_assemblers[Token::Type::Comment] = std::bind(
            &Lexer::processComment, this, std::placeholders::_1);
    _token_assemblers[Token::Type::String] = std::bind(
            &Lexer::processString, this, std::placeholders::_1);

    _context.push({skip_new_lines, skip_spaces});
}

Lexer::~Lexer() {
    if (_src_file.opened()) {
        _src_file.closeFile();
    }
}


Token::Token Lexer::nextToken() {
    std::cout << "Reading next token..." << "\n";
    while(!_src_file.eof()) {
        auto ch = _src_file.getNextChar();
        auto token_type = inferTokenTypeByFirstCharacter(ch);

        auto res = _token_assemblers[token_type](ch);

        if(skipToken(res)) {
            continue;
        }

        return res;
    }
    return {Token::Identifier::Nil, "", _src_file.line(), 0};
}


Token::Type Lexer::inferTokenTypeByFirstCharacter(const char ch) const {
    if(isBegginingOfTheIdentifier(ch)) {
        std::cout << "Infering ch as identifier\n";
        return Token::Type::Identifier;
    } else if(isDigit(ch)) {
        std::cout << "Infering as Const Expr\n";
        return Token::Type::ConstExpr;
    } else if(isPartOfOperator(ch)) {
        std::cout << "Infering ch as an operator\n";
        return Token::Type::Operator;
    } else if(isBlank(ch)) {
        std::cout << "Infering ch as blank\n";
        return Token::Type::Blank;
    } else if(isComment(ch)) {
        std::cout << "Infering ch as comment\n";
        return Token::Type::Comment;
    } else if(isString(ch)) {
        std::cout << "Infering ch as string\n";
        return Token::Type::String;
    }
    throw exception<Error>("Unknown character!");
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

bool Lexer::skipToken(const Token::Token &token) const {
    if(skipSpaces() && token.identifier() == Token::Identifier::Space) {
        return true;
    } else if (skipNewLines() && token.identifier() == Token::Identifier::NewLine) {
        return true;
    }
    return false;
}


Token::Token Lexer::processBlankChar(char ch) {
    std::cout << "Char is blank: " << ch << "\n";
    if(skipBlanks()) {
        std::cout << "Skipping blanks\n";
        _src_file.ungetChar(_src_file.getNextNonBlankChar());
    }
    if(isNewLine(ch)) {
        return newToken(Token::Identifier::NewLine, {ch});
    }
    return newToken(Token::Identifier::Space, {ch});
}

Token::Token Lexer::processConstExpr(char ch) {
    auto symbol = assembleConstExpr(ch);
    return newToken(Token::Identifier::ConstExpr, symbol);
}

Token::Token Lexer::processIdentifier(char ch) {
    auto symbol = assembleIdentifier(ch);
    if(_KEYWORDS.find(symbol) != _KEYWORDS.end()) {
        return newToken((*_KEYWORDS.find(symbol)).second, symbol);
    }
    return newToken(Token::Identifier::Identifier, symbol);

}

Token::Token Lexer::processComment(char ch) {
    auto symbol = assembleComment(ch);
    return newToken(Token::Identifier::Comment, symbol);
}

Token::Token Lexer::processOperator(char ch) {
    auto symbol = assembleOperator(ch);
    return newToken((*_OPERATORS.find(symbol)).second, symbol);
}

Token::Token Lexer::processString(char ch) {
    auto symbol = assembleString(ch);
    return newToken(Token::Identifier::String, symbol);
}


std::string Lexer::assembleConstExpr(char current) {
    std::cout << "Assembling Const Expr:\n";
    std::string symbol;
    do {
        if (isPartOfOperator(current) || isBlank(current)) {
            _src_file.ungetChar(current);
            return symbol;
        } else if (isDigit(current)) {
            symbol += current;
        } else {
            throw exception<ExpectedError>(std::string("digit"), std::string({current}));
        }
        current = _src_file.getChar();
        std::cout << "Read: " << current << "\n";
        std::cout << "Symbol: " << symbol << "\n";
    } while(!_src_file.eof());
    if(symbol.empty()) {
        throw exception<Error>("Could not assemble const expr!");
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
            _src_file.ungetChar(current);
            return symbol;
        } else {
            throw exception<ExpectedError>("[A-Za-z_0-9]", std::string({current}));
        }
        current = _src_file.getChar();
        std::cout << "Read: " << current << "\n";
        std::cout << "Symbol: " << symbol << "\n";
    } while(!_src_file.eof());
    if(symbol.empty()) {
        throw exception<Error>("Could not assemble identifier!");
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
                _src_file.ungetChar(current);
                return symbol;
            }
            symbol = guess;
        } else if (isBlank(current) || isPartOfIdentifier(current) ){
            _src_file.ungetChar(current);
            return symbol;
        } else {
            throw exception<ExpectedError>(std::string("operator"), std::string({current}));
        }
        current = _src_file.getChar();
        std::cout << "Read: " << current << "\n";
        std::cout << "Symbol: " << symbol << "\n";
    } while(!_src_file.eof());
    if (symbol.empty()) {
        throw exception<Error>("Could not assemble operator");
    }
    return symbol;
}

std::string Lexer::assembleComment(char current) {
    std::cout << "Assembling Comment\n";
    std::string symbol;
    do {
        if(isNewLine(current)) {
            _src_file.ungetChar(current);
            return symbol;
        }
        symbol += current;
        current = _src_file.getChar();
    } while(!_src_file.eof());
    if(symbol.empty()) {
        throw exception<Error>("Could not assemble comment!");
    }
    return symbol;
}

std::string Lexer::assembleString(char current) {
    std::cout << "Assembling string\n";
    std::string symbol;
    do {
        symbol += current;
        current = _src_file.getChar();
        if(isNewLine(current)) {
            throw exception<UnexpectedError>("new line");
        }
        if(isString(current)) {
            symbol += current;
            return symbol;
        }
    } while(!_src_file.eof());
    if(symbol.empty()) {
        throw exception<Error>("Could not assemble string!");
    }
    return symbol;
}


std::pair<bool, bool> Lexer::newContext(bool spaces, bool new_lines) {
    auto [_last_spaces, _last_new_lines] = _context.top();
    _context.push({spaces, new_lines});
    skip_new_lines = new_lines;
    skip_spaces = spaces;
    return {_last_spaces, _last_new_lines};
}

void Lexer::retrieveContext() {
    _context.pop();
    std::tie(skip_spaces, skip_new_lines) = _context.top();
}

Token::Token Lexer::newToken(Token::Identifier id, std::string symbol) const {
    std::cout << "Creating new token: {id: " << int(id) << " sym: " << symbol << "}\n";
    return Token::Token{
            id,
            std::move(symbol),
            _src_file.line()+1,
            _src_file.inLinePosition()};
}