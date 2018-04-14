#include <iostream>

#include "Source.h"
#include "Lexer/Lexer.h"
#include "Lexer/Token.h"
#include "Parser/Parser.h"

#include <sstream>
#include <string>

std::string tokenToStr(const Syntax::Token&);

int main() {
    Source src("test.txt");
    Syntax::Lexer lex(src);
    Parser::Parser pars(lex);

    auto tree = pars.parse();
    std::cout << "Got tree\n";
    std::cout << tree;

    std::cout << tree.getCurrent()->parse();
    return 0;
}

std::string tokenToStr(const Syntax::Token& t) {
    auto res = std::to_string(t.line()) + ":" + std::to_string(t.position());
    res += " TOKEN { type: " + std::to_string(int(t.type())) + " dentifier: " + std::to_string(int(t.identifier()));
    res += " symbol: \"" + t.symbol() + "\" }\n";
    return res;

}