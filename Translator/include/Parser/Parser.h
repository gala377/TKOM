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

        std::shared_ptr<Tree::Node> parseFunction();

        std::shared_ptr<FunctionDeclaration> parseFunctionDeclaration();
        std::string parseFunctionIdentifier();
        std::vector<std::string> parseFunctionArguments();

        std::vector<std::shared_ptr<Tree::Node>> parseCodeBlock(Scope& enveloping_scope);

        std::shared_ptr<Expression> parseVariableDeclaration(Scope& envelopingScope);
    };

}

#endif