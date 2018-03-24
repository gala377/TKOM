#include <iostream>
#include "Lexer/Lexer.h"
#include "Lexer/Token.h"

int main() {
    Syntax::Lexer lex;
    lex.loadFile("test.txt");
    Syntax::Token tok;
    do {
        tok = lex.nextToken();
        std::cout << tok.line() << ":" << tok.position() <<" Read token: Token{identifier: " << int(tok.identifier());
        std::cout << " symbol: " << tok.symbol() << "}\n";
    } while(tok.identifier() != Syntax::TokenIdentifier::Nil) ;
    return 0;
}