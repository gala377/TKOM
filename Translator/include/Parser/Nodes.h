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

        const std::string& symbol() const;

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

        std::string repr() const override;
    };


    class VariableCall:  public VariableDeclaration {
    public:
        using VariableDeclaration::Symbol;

        std::string parse() const override;
        std::string repr() const override;
    };


    class FunctionCall: public FunctionDeclaration {
    public:
        using FunctionDeclaration::FunctionDeclaration;

        std::string parse() const override;
        std::string repr() const override;
    };


    class Empty : public Expression {
    public:
        Empty();

        std::string parse() const;
        std::string repr() const;
    };
}

#endif //TRANSLATOR_NODES_H
