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
    std::string res = "Document\n";
    std::for_each(
            _children.begin(),
            _children.end(),
            [&res](auto& arg) {
                res += "\t" + arg->repr() + "\n";
            }
    );
    return res;
}


Function::Function(std::string identifier,
                   std::vector<std::string> args) : _identifier(std::move(identifier)), _args(std::move(args)) {};

std::string Function::parse() const {
    // for now just function, not prelude
    std::string res = "fn ";
    res += _identifier;
    res += "(";
    std::for_each(_args.begin(), _args.end()-1,
        [&res](auto node) { res += node + ", "; }
    );
    res += *_args.end() + " *int64)";
    // if return then return *int64
    return res;
}

std::string Function::repr() const {
    std::string res = "Function "+ _identifier +"\n";
    std::for_each(
            _children.begin(),
            _children.end(),
            [&res](auto& arg) {
                res += "\t" + arg->repr() + "\n";
            }
    );
    return res;
}