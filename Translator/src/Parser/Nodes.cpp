//
// Created by igor on 10.04.18.
//

#include "Parser/Nodes.h"
#include <algorithm>

using namespace Parser;


std::string Document::parse() const {
    std::string res = R"(package main

import "sync"
import "fmt"


)";
    std::for_each(
            _children.begin(),
            _children.end(),
            [&res](auto& arg) {
                res += arg->parse();
            }
    );
    return res;
}

std::string Document::repr() const {
    return "Document\n";
}



Symbol::Symbol(std::string symbol): _symbol(std::move(symbol)) {}

std::string Symbol::parse() const {
    return _symbol;
}

std::string Symbol::repr() const {
    return "Symbol " + _symbol + "\n";
}

const std::string& Symbol::symbol() const{
    return _symbol;
}



FunctionDeclaration::FunctionDeclaration(std::string identifier,
                   std::vector<std::string> args) : Symbol(std::move(identifier)),
                                                    _args(std::move(args)) {};

std::string FunctionDeclaration::parse() const {
    std::string res = funcPrelude();
    res += "\treturn ";
    res += returnedFunction();
    res += "\n}\n";
    res += "var " + _symbol + " = ";
    res += "__" + _symbol + "__CONSTRUCTOR__()\n\n";
    return res;
}

std::string FunctionDeclaration::funcPrelude() const {
    std::string res = "func ";
    res += "__" + _symbol + "__CONSTRUCTOR__() {\n";
    res += "\t// Function prelude\n";
    return res;
}

std::string FunctionDeclaration::returnedFunction() const {
    std::string res = "func ";
    res += "(";
    if (!_args.empty()) {
        std::for_each(_args.begin(), (_args.end() - 1),
                      [&res](auto &node) {
                          res += node + ", ";
                      });
        res += *(_args.end() - 1) + " *int64";
    }
    res += ") {\n";
    std::for_each(_children.begin(), _children.end(),
                  [&res](auto &arg) {
                      res += "\t\t" + arg->parse();
                  });
    res += "\n\t}";
    // if return then return *int64
    return res;
}

std::string FunctionDeclaration::repr() const {
    std::string res = "FunctionDeclaration "+ _symbol +"(";
    for(const auto& arg: _args) {
        res += arg + ", ";
    }
    res += ")\n";
    return res;
}

const std::vector<std::string>& FunctionDeclaration::args() const {
    return _args;
}




std::string VariableDeclaration::parse() const {
    return "var " + _symbol + " int64";
}

std::string VariableDeclaration::repr() const {
    return "variable " + _symbol +" declaration\n";
}


Empty::Empty(): Expression() {}

std::string Empty::parse() const {
    return "";
}

std::string Empty::repr() const {
    return "Empty\n";
}


Expression::Expression(): _left(nullptr), _right(nullptr) {
}

Expression::Expression(std::shared_ptr<Tree::Node> left_side,
                       std::shared_ptr<Tree::Node> right_side,
                       std::string root_operator): _left(left_side.get()),
                                                   _right(right_side.get()),
                                                   _operator(std::move(root_operator)) {
    addChild(left_side);
    addChild(right_side);
}

Expression::Expression(Tree::Node* left_side,
                       Tree::Node* right_side,
                       std::string root_operator): Expression(
                               std::shared_ptr<Tree::Node>(left_side),
                               std::shared_ptr<Tree::Node>(right_side),
                               std::move(root_operator)) {}

std::string Expression::parse() const {
    return _left->parse() + " " + _operator + " "+ _right->parse() + "\n";
}

std::string Expression::repr() const {
    return "Expression " + _operator +"\n";
}

Tree::Node* Expression::left() {
    return _left;
}

Tree::Node* Expression::right() {
    return _right;
}


InBracketExpr::InBracketExpr(std::shared_ptr<Expression> expr): Expression(
        std::move(expr),
        std::make_shared<Empty>(),
        "") {}

std::string InBracketExpr::parse() const {
    return "(" + Expression::parse() + ")";
}

std::string InBracketExpr::repr() const {
    return "Brackets ()";
}

std::shared_ptr<Expression> InBracketExpr::expr() const {
    return _expr;
}



Assignment::Assignment(std::shared_ptr<Tree::Node> left_side,
                       std::shared_ptr<Tree::Node> right_side): Expression(
                               std::move(left_side),
                               std::move(right_side),
                               "=") {}

Assignment::Assignment(Tree::Node* left_side, Tree::Node* right_side): Expression(left_side,
                                                                                right_side,
                                                                                "=") { }

std::string Assignment::repr() const {
    return "Assignment\n";
}



std::string VariableCall::parse() const {
    return symbol();
}

std::string VariableCall::repr() const {
    return "Call var " + symbol();
}



std::string FunctionCall::parse() const {
    std::string res = "func ";
    res += "(";
    if (!_args.empty()) {
        std::for_each(_args.begin(), (_args.end() - 1),
                      [&res](auto &node) {
                          res += node + ", ";
                      });
        res += *(_args.end() - 1) + " *int64";
    }
    res += ")";
    return res;
}

std::string FunctionCall::repr() const {
    return "call " + FunctionDeclaration::repr();
}


ConstExpr::ConstExpr(std::string symbol): Expression(),
                                          Symbol(std::move(symbol)) {}

std::string ConstExpr::parse() const {
    return Symbol::parse();
}

std::string ConstExpr::repr() const {
    return "ConstExpr " + Symbol::repr();
}

