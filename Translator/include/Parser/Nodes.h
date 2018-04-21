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

    class Declaration: public Tree::Node {
    public:
        Declaration(std::string identifier);

        const std::string& identifier() const;

        std::string parse() const override;
        std::string repr() const override;

    protected:
        std::string _identifier;
    };

    class Function : public Declaration {
    public:
        Function(std::string identifier,
                 std::vector<std::string> args);

        std::string parse() const override;
        std::string repr() const override;

        const std::vector<std::string>& args() const;
    protected:
        std::vector<std::string> _args;

        std::string funcPrelude() const ;
        std::string returnedFunction() const ;
    };

    class VariableDeclaration: public Declaration {
    public:
        using Declaration::Declaration;

        std::string parse() const override;
        std::string repr() const override;
    };

    class Assigment: public Tree::Node {
    public:
        Assigment(Tree::Node* left_side, Tree::Node* right_side);

        std::string parse() const override;
        std::string repr() const override;

        Tree::Node* left();
        Tree::Node* right();

    private:
        Tree::Node* _left;
        Tree::Node* _right;
    };

    class Empty : public Tree::Node {
        std::string parse() const;
        std::string repr() const;
    };
}

#endif //TRANSLATOR_NODES_H
