#include <iostream>
#include "Lexer/Lexer.h"
#include "Lexer/Token.h"
#include <sstream>
#include <string>

std::string tokenToStr(const Syntax::Token&);

int main() {
    Syntax::Lexer lex;
    lex.skip_spaces = false;
    lex.skip_new_lines = false;
    lex.loadFile("test.txt");
    std::vector<std::string> stats;
    Syntax::Token tok;
    do {
        tok = lex.nextToken();
        std::cout << tokenToStr(tok);
        stats.push_back(tokenToStr(tok));
    } while(tok.identifier() != Syntax::TokenIdentifier::Nil) ;

    std::cout << "\n\n\n\nRESULTS:\n";
    for(const auto& s: stats) {
        std::cout << s;
    }

    return 0;
}

std::string tokenToStr(const Syntax::Token& t) {
    auto res = std::to_string(t.line()) + ":" + std::to_string(t.position());
    res += " TOKEN { type: " + std::to_string(int(t.type())) + " dentifier: " + std::to_string(int(t.identifier()));
    res += " symbol: \"" + t.symbol() + "\" }\n";
    return res;

}