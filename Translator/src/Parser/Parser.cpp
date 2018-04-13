#include "Parser/Parser.h"
#include "Lexer/Token.h"
#include "Parser/Nodes.h"

#include <iostream>

using namespace Parser;

Parser::Parser::Parser(Syntax::Lexer& lexer) : _lexer(lexer) {
    auto root = new Document;
    _tree = Tree(root);
};

Tree Parser::Parser::parse() {
    Syntax::Token curr;
    _lexer.skip_new_lines = true;
    _lexer.skip_spaces = true;
    for(auto curr = _lexer.nextToken();
        curr.type() != Syntax::TokenType::Nil;
        curr = _lexer.nextToken()) {
        
        if(curr.type() == Syntax::TokenType::Comment) {
            continue;
        } else if(curr.identifier() == Syntax::TokenIdentifier::Function) {
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
    auto function = assembleFunctionDeclaration();
    auto curr = _lexer.nextToken();
    if(curr.type() == Syntax::TokenType::Nil) {
        throw std::runtime_error("Function body expected got EOF!");
    }
    function->addChild( parseFunctionBodyInstruction() );
    return function;
}

Function* Parser::Parser::assembleFunctionDeclaration() {
    _lexer.skip_new_lines = false;
    _lexer.skip_spaces = false;
    std::string identifier;
    std::vector<std::string> args;

    if (auto curr = _lexer.nextToken(); curr.identifier() != Syntax::TokenIdentifier::Space) {
        throw std::runtime_error("Expected single space");
    }
    if (auto curr = _lexer.nextToken(); curr.identifier() == Syntax::TokenIdentifier::Identifier) {
        if(_identifiers.count(curr.symbol())) {
            throw std::runtime_error("Identifier already used!");
        }
        _identifiers.insert(curr.symbol());
        identifier = curr.symbol();
    } else {
        throw std::runtime_error("Expected function name");
    }
    args = parseFunctionArguments();
    return new Function(identifier, args);
}

std::vector<std::string> Parser::Parser::parseFunctionArguments() {
    _lexer.skip_spaces = true;
    _lexer.skip_new_lines = true;
    std::vector<std::string> args;
    if(auto curr = _lexer.nextToken(); curr.identifier() != Syntax::TokenIdentifier::OpenBracket) {
        throw std::runtime_error("Expected open bracket after function declaration!");
    }
    switch(auto curr = _lexer.nextToken(); curr.identifier()) {
        case Syntax::TokenIdentifier::CloseBracket:
        break;
        case Syntax::TokenIdentifier::Identifier:
        args.push_back(curr.symbol());
        for(curr = _lexer.nextToken();
         curr.identifier() != Syntax::TokenIdentifier::CloseBracket;
         curr = _lexer.nextToken()) {
             if(curr.identifier() != Syntax::TokenIdentifier::Comma) {
                 throw std::runtime_error("Comma expected!");
             }
             if(curr = _lexer.nextToken(); curr.identifier() != Syntax::TokenIdentifier::Identifier) {
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

Tree::Node* Parser::Parser::parseFunctionBodyInstruction() {
    _lexer.skip_spaces = true;
    _lexer.skip_new_lines = true;
    std::cout << "Skipping function body!\n";
    for (auto curr = _lexer.nextToken();
         curr.identifier() != Syntax::TokenIdentifier::CloseCurlyBracket;
         curr = _lexer.nextToken()) {
        std::cout << curr.symbol();
    }
    return new Empty();
}