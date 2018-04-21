#include "Parser/Parser.h"
#include "Lexer/Token.h"
#include "Parser/Nodes.h"

#include <iostream>

using namespace Parser;

// TODO no main function is an error

Parser::Parser::Parser(Syntax::Lexer& lexer) : _lexer(lexer), _scope(Scope()) {
    auto root = new Document;
    _tree = Tree(root);
};

Tree Parser::Parser::parse() {
    _lexer.newContext(true, true);
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
    _lexer.retrieveContext();
    return std::move(_tree);
}


Tree::Node* Parser::Parser::parseFunction() {
    auto function = parseFunctionDeclaration();
    auto function_scope = _scope.newSubScope();
    for(auto arg: function->args()) {
        function_scope.addIdentifier({arg, Identifier::variable});
    }
    for(auto child: parseCodeBlock(function_scope)) {
        function->addChild(child);
    }
    return function;
}

Function* Parser::Parser::parseFunctionDeclaration() {
    auto identifier = parseFunctionIdentifier();
    auto args = parseFunctionArguments();
    return new Function(identifier, args);
}

std::string Parser::Parser::parseFunctionIdentifier() {
    _lexer.newContext(true, false);
    if (auto curr = _lexer.nextToken(); curr.identifier() == Syntax::Token::Identifier::Identifier) {
        if(_scope.isDefined(curr.symbol())) {
            throw std::runtime_error("Identifier already used!");
        }
        _scope.addIdentifier({curr.symbol(), Identifier::function});
        _lexer.retrieveContext();
        return curr.symbol();
    } else {
        throw std::runtime_error("Expected function name");
    }
}

std::vector<std::string> Parser::Parser::parseFunctionArguments() {
    _lexer.newContext(true, true);
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
    _lexer.retrieveContext();
    return args;
}


std::vector<Tree::Node*> Parser::Parser::parseCodeBlock(Scope& enveloping_scope) {
    _lexer.newContext(true, true);

    std::vector<Tree::Node*> expressions;
    auto current_scope = enveloping_scope.newSubScope();

    for (auto curr = _lexer.nextToken();
         curr.identifier() != Syntax::Token::Identifier::CloseCurlyBracket;
         curr = _lexer.nextToken()) {

        if(curr.identifier() == Syntax::Token::Identifier::Comment) {
            continue;
        } else if(curr.identifier() == Syntax::Token::Identifier::Variable) {
            expressions.push_back(parseVariableDeclaration(current_scope));
        }
    }

    _lexer.retrieveContext();
    return expressions;
}

Assigment* Parser::Parser::parseVariableDeclaration(Scope& enveloping_scope) {
    _lexer.newContext(true, false);
    auto curr = _lexer.nextToken();

    if(curr.identifier() != Syntax::Token::Identifier::Identifier) {
        throw std::runtime_error("Expected identifier after variable declaration!");
    }
    if(enveloping_scope.isDefined(curr.symbol())) {
        throw std::runtime_error("Symbol already in use!");
    }
    enveloping_scope.addIdentifier({curr.symbol(), Identifier::variable});
    auto left_side = new VariableDeclaration(curr.symbol());

    curr = _lexer.nextToken();
    if(curr.identifier() != Syntax::Token::Identifier::Assigment) {
        throw std::runtime_error("Assigments operator expected!");
    }
    // TODO parse expression

    _lexer.retrieveContext();
    return new Assigment(left_side, new Empty());
}
