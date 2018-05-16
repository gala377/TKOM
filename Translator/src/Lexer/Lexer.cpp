//
// Created by igor on 24.03.18.
//

#include <iostream>
#include <Logger/Logger.h>

#include "Lexer/Lexer.h"
#include "Lexer/Characters.h"
#include "Lexer/Exceptions.h"

using namespace Syntax;

// TODO make it so the identifiers and the rest have max lenght

SymbolMap Lexer::_KEYWORDS = {
        {"fn", Token::Identifier::Function},
        {"loop", Token::Identifier::Loop},
        {"if", Token::Identifier::If},
        {"else", Token::Identifier::Else},
        {"critical", Token::Identifier::Critical},
        {"concurrent", Token::Identifier::Concurrent},
        {"let", Token::Identifier::Variable},
        {"return", Token::Identifier::Return},
        {"log", Token::Identifier::Print},
};


SymbolMap Lexer::_OPERATORS = {
        {"==", Token::Identifier::Equality},
        {"!=", Token::Identifier::NonEquality},
        {"+", Token::Identifier::Addition},
        {"*", Token::Identifier::Multiplication},
        {"/", Token::Identifier::Division},
        {"=", Token::Identifier::Assignment},
        {">", Token::Identifier::Greatness},
        {"<", Token::Identifier::Minority},
        {"(", Token::Identifier::OpenBracket},
        {")", Token::Identifier::CloseBracket},
        {"{", Token::Identifier::OpenCurlyBracker},
        {"}", Token::Identifier::CloseCurlyBracket},
        {",", Token::Identifier::Comma},
        {"#", Token::Identifier::Comment},
        {"-", Token::Identifier::Minus},
        {"!", Token::Identifier::Negation},
};


Lexer::Lexer(Source& src, Logging::Logger logger): _src_file(src), _logger(logger) {
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


std::tuple<int, int> Lexer::inFilePosition() const {
    return {_src_file.line(), _src_file.inLinePosition()};
}

Token::Token Lexer::nextToken() {
    _logger << "Reading next token..." << "\n";
    if(!_pending_tokens.empty()) {
        auto ret_token = _pending_tokens.top();
        _pending_tokens.pop();
        return ret_token;
    }
    while(!_src_file.eof()) {
        auto ch = _src_file.getNextChar();
        auto token_type = inferTokenTypeByFirstCharacter(ch);

        auto res = _token_assemblers[token_type](ch);

        if(skipToken(res)) {
            continue;
        }

        return res;
    }
    return {Token::Identifier::Eof, "", _src_file.line(), 0};
}

void Lexer::ungetToken(Token::Token token) {
    _pending_tokens.push(token);
}

Token::Type Lexer::inferTokenTypeByFirstCharacter(const char ch) const {
    if(isBegginingOfTheIdentifier(ch)) {
        return Token::Type::Identifier;
    } else if(isDigit(ch)) {
        return Token::Type::ConstExpr;
    } else if(isPartOfOperator(ch)) {
        return Token::Type::Operator;
    } else if(isBlank(ch)) {
        return Token::Type::Blank;
    } else if(isComment(ch)) {
        return Token::Type::Comment;
    } else if(isString(ch)) {
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
    _logger << "Char is blank: " << ch << "\n";
    if(skipBlanks()) {
        _logger << "Skipping blanks\n";
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
    _logger << "Assembling Const Expr:" << "\n";
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
        _logger << "Read: " << current << "\n";
        _logger << "Symbol: " << symbol << "\n";
    } while(!_src_file.eof());
    if(symbol.empty()) {
        throw exception<Error>("Could not assemble const expr!");
    }
    return symbol;
}

std::string Lexer::assembleIdentifier(char current) {
    _logger << "Assembling Identifier:" << "\n";
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
        _logger << "Read: " << current << "\n";
        _logger << "Symbol: " << symbol << "\n";
    } while(!_src_file.eof());
    if(symbol.empty()) {
        throw exception<Error>("Could not assemble identifier!");
    }
    return symbol;
}

std::string Lexer::assembleOperator(char current) {
    _logger << "Assembling Operator" << "\n";
    std::string symbol;
    do {
        if (isPartOfOperator(current)) {
            auto guess = symbol + current;
            _logger << "Current guess: " << guess << "\n";
            if (_OPERATORS.find(guess) == _OPERATORS.end()) {
                _src_file.ungetChar(current);
                return symbol;
            }
            symbol = guess;
        } else if (isBlank(current) || isPartOfIdentifier(current) || isString(current)){
            _src_file.ungetChar(current);
            return symbol;
        } else {
            throw exception<ExpectedError>(std::string("operator"), std::string({current}));
        }
        current = _src_file.getChar();
        _logger << "Read: " << current << "\n";
        _logger << "Symbol: " << symbol << "\n";
    } while(!_src_file.eof());
    if (symbol.empty()) {
        throw exception<Error>("Could not assemble operator");
    }
    return symbol;
}

std::string Lexer::assembleComment(char current) {
    _logger << "Assembling Comment" << "\n";
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
    _logger << "Assembling string" << "\n";
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
    return Token::Token{
            id,
            std::move(symbol),
            _src_file.line()+1,
            _src_file.inLinePosition()};
}