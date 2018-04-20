#include "Parser/Parser.h"
#include "Lexer/Token.h"
#include "Parser/Nodes.h"

#include <iostream>

using namespace Parser;

// TODO ni main function is an error
// TODO check identifiers

Parser::Parser::Parser(Syntax::Lexer& lexer) : _lexer(lexer) {
    auto root = new Document;
    _tree = Tree(root);
};

Tree Parser::Parser::parse() {
    _lexer.skip_new_lines = true;
    _lexer.skip_spaces = true;
    for(auto curr = _lexer.nextToken();
        curr.type() != Syntax::Token::Type::Nil;
        curr = _lexer.nextToken()) {
        
        if(curr.type() == Syntax::Token::Type::Comment) {
            continue;
        } else if(curr.identifier() == Syntax::Token::Identifier::Function) {
            _tree.getCurrent()->addChild( parseFunction() );
        } else {
            // TODO - throw custom exception with wrong token info inside
            throw std::runtime_error("Expected comment or function got something else");
        }
    }
    std::cout << "Returning tree\n";
    return _tree;
}

Tree::Node* Parser::Parser::parseFunction() {
    auto function = parseFunctionDeclaration();
    // Make it range loop which adds children
    function->addChild( parseCodeBlock() );
    return function;
}

Function* Parser::Parser::parseFunctionDeclaration() {
    auto identifier = parseFunctionIndentifier();
    auto args = parseFunctionArguments();
    return new Function(identifier, args);
}

std::string Parser::Parser::parseFunctionIndentifier() {
    _lexer.skip_spaces = true;
    _lexer.skip_new_lines = false;
    if (auto curr = _lexer.nextToken(); curr.identifier() == Syntax::Token::Identifier::Identifier) {
        if(_identifiers.count(curr.symbol())) {
            throw std::runtime_error("Identifier already used!");
        }
        _identifiers.insert(curr.symbol());
        return curr.symbol();
    } else {
        throw std::runtime_error("Expected function name");
    }
}

std::vector<std::string> Parser::Parser::parseFunctionArguments() {
    _lexer.skip_spaces = true;
    _lexer.skip_new_lines = true;
    std::vector<std::string> args;
    if(auto curr = _lexer.nextToken(); curr.identifier() != Syntax::Token::Identifier::OpenBracket) {
        throw std::runtime_error("Expected open bracket after function identifier!");
    }
    switch(auto curr = _lexer.nextToken(); curr.identifier()) {
        case Syntax::Token::Identifier::CloseBracket:
        break;
        case Syntax::Token::Identifier::Identifier:
        args.push_back(curr.symbol());
        for(curr = _lexer.nextToken();
         curr.identifier() != Syntax::Token::Identifier::CloseBracket;
         curr = _lexer.nextToken()) {
             if(curr.identifier() != Syntax::Token::Identifier::Comma) {
                 throw std::runtime_error("Comma expected!");
             }
             if(curr = _lexer.nextToken(); curr.identifier() != Syntax::Token::Identifier::Identifier) {
                 throw std::runtime_error("Identifier expected!");
             }
             args.push_back(curr.symbol());
         }
        break; 
        default:
        throw std::runtime_error("Identifier or ')' expected.");
    }
    return args;
}

Tree::Node* Parser::Parser::parseCodeBlock() {
    _lexer.skip_spaces = true;
    _lexer.skip_new_lines = true;
    std::cout << "Skipping function body!\n";
    for (auto curr = _lexer.nextToken();
         curr.identifier() != Syntax::Token::Identifier::CloseCurlyBracket;
         curr = _lexer.nextToken()) {
        std::cout << curr.symbol();
    }
    return new Empty();
}