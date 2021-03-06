#include <iostream>

#include "Source.h"
#include "Lexer/Lexer.h"
#include "Lexer/Token.h"
#include "Parser/Parser.h"
#include "Logger/Logger.h"
#include "Translator/Go.h"

#include <sstream>
#include <string>

std::string tokenToStr(const Syntax::Token::Token&);

int main(int argv, char* argc[]) {
    Source src(argc[1]);
    Logging::Logger lexer_log(std::cout, "[Lexer]: ");
    Syntax::Lexer lex(src, lexer_log);

    Logging::Logger log(std::cout, "[Parser]: ");
    Parser::Parser pars(lex, log);

    auto tree = pars.parse();
    std::cout << "Got tree\n";
    std::cout << tree;

    std::cout << tree.getCurrent()->parse();

    Translation::Go translator;
    std::cout << "\nTRANSLATOR OUTPUT:\n";
    std::cout << translator.translate(tree);

    return 0;
}

std::string tokenToStr(const Syntax::Token::Token& t) {
    auto res = std::to_string(t.line()) + ":" + std::to_string(t.position());
    res += " TOKEN { type: " + std::to_string(int(t.type())) + " identifier: " + std::to_string(int(t.identifier()));
    res += " symbol: \"" + t.symbol() + "\" }\n";
    return res;

}