//
// Created by igor on 10.04.18.
//

#ifndef TRANSLATOR_NODES_H
#define TRANSLATOR_NODES_H

#include "Parser/Tree.h"

namespace Parser {

    class Document : public Tree::Node {
        std::string parse() const;
        std::string repr() const;
    };

    class Function : public Tree::Node {
    public:
        Function(std::string identifier,
                 std::vector<std::string> args);

        std::string parse() const;
        std::string repr() const;

    protected:
        std::string _identifier;
        std::vector<std::string> _args;

        std::string funcPrelude() const ;
        std::string returnedFunction() const ;
    };

    class Empty : public Tree::Node {
    public:
        std::string parse() const;
        std::string repr() const; 
    };

}

#endif //TRANSLATOR_NODES_H
