#ifndef __PARSER__H__
#define __PARSER__H__

#include "Lexer/Lexer.h"
#include "Parser/Tree.h"
#include "Parser/Nodes.h"
#include "Parser/Scope.h"
#include "Logger/Logger.h"
#include <set>

namespace Parser {

    class Parser {
        public:
        using token_id_t = Syntax::Token::Identifier;
        using token_type_t = Syntax::Token::Type;

        Parser(Syntax::Lexer& lexer, Logging::Logger logger = Logging::Logger());

        Tree parse();

        private:
        Syntax::Lexer& _lexer;

        Tree _tree;
        Scope _scope;

        const std::set<token_id_t> _EXPR_OPERATORS = {
                token_id_t::Addition, token_id_t::Minus,
                token_id_t::Multiplication, token_id_t::Division,
                token_id_t::Minority, token_id_t::Greatness,
                token_id_t::Equality, token_id_t::NonEquality,
        };

        Logging::Logger _logger;

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
        bool isOperatorValidInExpr(token_id_t op) const;

        std::shared_ptr<Expression> parseFunctionCall(Scope& enveloping_scope);
        FunctionCall::args_t parseFunctionParameters(Scope& enveloping_scope);

        std::shared_ptr<Expression> parsePrintCall(Scope& enveloping_scope);
        PrintCall::args_t parsePrintParameters(Scope& enveloping_scope);

        // Statements

        std::shared_ptr<Statement> parseCodeBlockStatement(Scope& enveloping_scope);
        std::shared_ptr<Statement> parseReturn(Scope& enveloping_scope);
        std::shared_ptr<Statement> parseLoop(Scope& enveloping_scope);
        std::shared_ptr<Statement> parseIf(Scope& enveloping_scope);
        std::shared_ptr<ElseStatement> parseElse(Scope& enveloping_scope);
        std::shared_ptr<Statement> parseCritical(Scope& enveloping_scope);
        std::shared_ptr<Statement> parseConcurrent(Scope& enveloping_scope);

        template <class ...Args>
        void log(Args&&... args) {
            std::cout << "[Parser]: ";
            (std::cout <<  ... << args) << "\n";
        }

        template <class Exception, class ...Args>
        Exception exception(Args... args) const {
            auto [line, position] = _lexer.inFilePosition();
            return Exception(line, position, args...);
        };
    };



}

#endif