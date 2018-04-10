#ifndef __PARSER__H__
#define __PARSER__H__

#include "Lexer/Lexer.h"
#include "Parser/Tree.h"
#include <set>

namespace Parser {

    class Parser {
        public:

        Parser(Syntax::Lexer& lexer);

        Tree parse();

        private:
        Syntax::Lexer& _lexer;
        Tree _tree;

        // TODO change to hash set
        std::set<std::string> _identifiers;

        Node* parseFunction(Syntax::Token& curr);
        bool validateFunctionDeclaration(Syntax::Token& curr);

        Node* parseFunctionBodyInstruction(Syntax::Token curr);
    };

}

#endif