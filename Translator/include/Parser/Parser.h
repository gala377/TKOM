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

        // Expressions

        std::shared_ptr<Expression> parseVariableDeclaration(Scope& enveloping_scope);
        std::shared_ptr<Expression> parseExpression(Scope& enveloping_scope);
        std::shared_ptr<Expression> parseBracketExpression(Scope& enveloping_scope);

        std::shared_ptr<Expression> parseLeftSideOfExpr(Scope& enveloping_scope);

        // Statements

        std::shared_ptr<Statement> parseReturn(Scope& enveloping_scope);
        std::shared_ptr<Statement> parseLoop(Scope& enveloping_scope);
        std::shared_ptr<Statement> parseIf(Scope& enveloping_scope);
        std::shared_ptr<Statement> parseCritical(Scope& enveloping_scope);
        std::shared_ptr<Statement> parseConcurrent(Scope& enveloping_scope);

        template <class ...Args>
        void log(Args&&... args) {
            std::cout << "[Parser]: ";
            (std::cout <<  ... << args) << "\n";
        }
    };



}

#endif