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


Function::Function(std::string identifier,
                   std::vector<std::string> args) : _identifier(std::move(identifier)), _args(std::move(args)) {};

std::string Function::parse() const {
    std::string res = funcPrelude();
    res += "\treturn ";
    res += returnedFunction();
    res += "\n}\n";
    res += "var " + _identifier + " = ";
    res += "__" + _identifier + "__CONSTRUCTOR__()\n\n";
    return res;
}

std::string Function::funcPrelude() const {
    std::string res = "func ";
    res += "__" + _identifier + "__CONSTRUCTOR__() {\n";
    res += "\t// Function prelude\n";
    return res;
}

std::string Function::returnedFunction() const {
    std::string res = "func ";
    res += "(";
    if (_args.size())
    {
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

std::string Function::repr() const {
    std::string res = "Function "+ _identifier +"(";
    for(auto arg: _args) {
        res += arg + ", ";
    }
    res += ")\n";
    return res;
}

std::string Empty::parse() const {
    return "";
}

std::string Empty::repr() const {
    std::cout << "Empty repr call\n";
    return "Empty\n";
}