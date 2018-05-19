//
// Created by igor on 10.04.18.
//

#ifndef TRANSLATOR_NODES_H
#define TRANSLATOR_NODES_H

#include "Parser/Tree.h"

namespace Parser {

    class Document : public Tree::Node {
        std::string parse() const override;
        std::string repr() const override;
    };



    class Symbol: public virtual Tree::Node {
    public:
        Symbol(std::string symbol);

        std::string parse() const override;
        std::string repr() const override;

        virtual const std::string& symbol() const;

    protected:
        std::string _symbol;
    };



    class FunctionDeclaration : public Symbol {
    public:
        FunctionDeclaration(std::string identifier,
                 std::vector<std::string> args);

        std::string parse() const override;
        std::string repr() const override;

        const std::vector<std::string>& args() const;
    protected:
        std::vector<std::string> _args;

        std::string funcPrelude() const ;
        std::string returnedFunction() const ;
    };


    class VariableDeclaration: public Symbol {
    public:
        using Symbol::Symbol;

        std::string parse() const override;
        std::string repr() const override;
    };



    class Expression: public virtual Tree::Node {
    public:
        // The one below is unsafe and should be used with caution
        // Look at Empty definition
        Expression();
        Expression(Tree::Node* left_side, Tree::Node* right_side, std::string root_operator);
        Expression(std::shared_ptr<Node> left_side,
                   std::shared_ptr<Node> right_side,
                   std::string root_operator);

        std::string parse() const override;
        std::string repr() const override;

        Tree::Node* left();
        Tree::Node* right();

    protected:
        Tree::Node* _left;
        Tree::Node* _right;

        std::string _operator;
    };


    class ConstExpr: public Expression, Symbol {
    public:
        ConstExpr(std::string symbol);

        std::string parse() const override;
        std::string repr() const override;
    };


    class InBracketExpr: public Expression {
    public:
        explicit InBracketExpr(std::shared_ptr<Expression> expr);

        std::string parse() const override;
        std::string repr() const override;

        std::shared_ptr<Expression> expr() const;
    protected:
        std::shared_ptr<Expression> _expr;
    };


    class Assignment: public Expression {
    public:
        Assignment(Tree::Node* left_side, Tree::Node* right_side);
        Assignment(std::shared_ptr<Tree::Node> left_side,
                   std::shared_ptr<Tree::Node> right_side);

        std::string parse() const override;
        std::string repr() const override;
    };



    class VariableCall:  public Expression, public VariableDeclaration {
    public:
        VariableCall(std::string symbol);

        std::string parse() const override;
        std::string repr() const override;
    };


    class FunctionCall: public Expression, public FunctionDeclaration {
    public:
        using args_t = std::vector<std::string>;

        FunctionCall(std::string symbol, args_t args);

        std::string parse() const override;
        std::string repr() const override;
    };


    class LibraryFunctionCall: public FunctionCall {
    public:
        LibraryFunctionCall(std::string library, std::string symbol, args_t args);

        std::string library() const;
        std::string parse() const override;

    protected:
        std::string _library;
    };


    class PrintCall: public FunctionCall {
    public:
        explicit PrintCall(args_t args);
    };



    class Statement: public Symbol {
    public:
        explicit Statement(std::string symbol);
        Statement(std::string symbol, std::shared_ptr<Expression> expr);

        std::string parse() const override;
        std::string repr() const override;

        std::shared_ptr<Expression> expr() const;

    protected:
        std::shared_ptr<Expression> _expr;

    };


    class BlockStatement: public Statement {
    public:

        using block_t = std::vector<std::shared_ptr<Tree::Node>>;

        explicit BlockStatement(block_t block);
        BlockStatement(std::string symbol,
                       block_t block);
        BlockStatement(std::string symbol,
                       std::shared_ptr<Expression> expr,
                       block_t block);

        std::string parse() const override;
    };


    class ElseStatement: public Statement {
    public:
        explicit ElseStatement(std::shared_ptr<BlockStatement> after_else_clause);

        std::string parse() const override;
    };


    class IfStatement: public BlockStatement {
    public:
        IfStatement(std::shared_ptr<Expression> expr,
                    block_t block);

        void setElse(std::shared_ptr<ElseStatement> else_statement);

        std::string parse() const override;

    private:
        std::shared_ptr<ElseStatement> _else;
    };



    class Empty : public Expression {
    public:
        Empty();

        std::string parse() const override;
        std::string repr() const override;
    };
}

#endif //TRANSLATOR_NODES_H
