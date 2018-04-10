#include "Parser/Parser.h"


using namespace Parser;

Parser::Parser(Syntax::Lexer& lexer): _lexer(lexer) {};

Tree Parser::parse() {
    Syntax::Token curr;
    lexer.skip_new_lines = true;
    lexer.SKip_spaces = true;
    do {
        curr = lexer.nextToken();
        if(curr.type == Syntax::TokenType::Comment) {
            continue;
        } else if(curr.identifier == Syntax::TokenIdentifier::Function) {
            tree.addChild( parseFunction(curr) );
        } else {
            // TODO - throw custom exception with wrong token info inside
            throw std::runtime_error("Expected comment or function got something else");
        }
    } while(curr.type != Syntax::TokenType::Nil)
}

Node* Parser::parseFunction(Syntax::Token& curr) {
    Node* function = new Node;
    validateFunctionDeclaration(curr);
    // Expect end of file, if so throw !
    curr = _lexer.nextToken();
    if(curr.type == Syntax::TokenType::Nil) {
        throw std::runtime_error("Function body expected got EOF!");
    }
    while(curr.identifier != Syntax::TokenIdentifier::CloseCurlyBracket) {
        function.addChild( parseFunctionBodyInstruction(curr) );
    }
    return function;
}

bool Parser::validateFunctionDeclaration(Syntax::Token& curr) {
    _lexer.skip_new_lines = false;
    _lexer.skip_spaces = false;
    curr = _lexer.nextToken();
    if(curr.identifier != Syntax::TokenIdentifier::Space) {
        throw std::runtime_error("Expected single space");
    }
    curr = _lexer.nextToken();
    if(curr.identifier == Syntax::TokenIdentifier::Identifier) {
        if(_identifiers.count(curr.symbol)) {
            throw std::runtime_error("Identifier already used!");
        }
        _identifiers.insert(curr.symbol);
    } else {
        throw std::runtime_error("Expected function name");
    }
    // Todo parse argument list

    _lexer.skip_new_lines = true;
    _lexer.skip_spaces = true;
    return true;
}