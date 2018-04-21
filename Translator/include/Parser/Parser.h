#ifndef __PARSER__H__
#define __PARSER__H__

#include "Lexer/Lexer.h"
#include "Parser/Tree.h"
#include "Parser/Nodes.h"
#include "Parser/Scope.h"
#include <set>

namespace Parser {

    class Parser {
        public:
        Parser(Syntax::Lexer& lexer);

        Tree parse();

        private:
        Syntax::Lexer& _lexer;

        Tree _tree;
        Scope _scope;

        Tree::Node* parseFunction();

        Function* parseFunctionDeclaration();
        std::string parseFunctionIndentifier();
        std::vector<std::string> parseFunctionArguments();

        Tree::Node* parseCodeBlock();
    };

}

#endif