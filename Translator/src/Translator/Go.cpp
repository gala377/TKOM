//
// Created by igor on 19.05.18.
//

#include <cassert>
#include <string>
#include <iostream>

#include <Parser/Nodes.h>
#include "Translator/Go.h"


std::string Translation::Go::translate(Parser::Tree& abstr_tree) {
    abstr_tree.goToRoot();
    auto res = parseDocument(abstr_tree.getCurrent());
    if(!_main_func_present) {
        throw std::runtime_error("Expected main function definition");
    }
    return res;
}

std::string Translation::Go::parseDocument(Translation::Go::Node_ptr node) {
    std::string document;
    for(const auto& child: node->getChildren()) {
        document += parseFunctionDeclaration(child.get());
    }

    document = documentHeader() + document;
    return document;
}

std::string Translation::Go::documentHeader() {
    std::string head = "package main\n\n";
    if(_add_fmt_import) {
        head += R"(import "fmt")";
        head += "\n";
    }
    if(_add_sync_import) {
        head += R"(import "sync")";
        head += "\n\n";
    }
    return head;
}

std::string Translation::Go::parseFunctionDeclaration(Translation::Go::Node_ptr node) {
    Parser::FunctionDeclaration* func;
    if((func = dynamic_cast<Parser::FunctionDeclaration*>(node)) == nullptr) {
        throw std::runtime_error("FunctionDeclaration node expected!");
    }

    assertFlagsAreZeroed();
    if(func->symbol() == "main") {
        _main_func_present = true;
        return parseMain(func);
    }


    std::cout << "NOT MAIN FUNCTION\n";
    auto header = functionHeader(func);

    incIntend();
    auto body = returnedFunction(func);
    auto prelude = functionPrelude(func);
    decIntend();
    auto constructor = funcConstructor(func);

    return header + "{\n" + prelude + body + "}\n" + constructor;
}

void Translation::Go::assertFlagsAreZeroed() {
    assert(!_return_at_the_end_of_func);
    assert(!_critical_count);
}

std::string Translation::Go::functionHeader(ptr_t<Parser::FunctionDeclaration> func) {
    std::string header = "__func__" + func->symbol() + "__CONSTRUCTOR__()";
    // return type
    header += " func(";
    for(const auto& arg: func->args()) {
        header += "int64, ";
    }
    if(header[header.length()-2] == ',') {
        header = header.substr(0, header.length()-2);
    }
    header += ") int64 ";
    return header;

}

std::string Translation::Go::functionPrelude(ptr_t<Parser::FunctionDeclaration> func) {
    if(_critical_count > 0) {
        _add_sync_import = true;
    }
    std::string prelude = addIntend("return ");
    return prelude;
}

std::string Translation::Go::returnedFunction(ptr_t<Parser::FunctionDeclaration> func) {
    std::string returned = "func(";
    for(const auto& arg: func->args()) {
        returned += arg + ", ";
    }
    if(returned[returned.length()-2] == ',') {
        std::cout << "ENDING WITH ,";
        returned = returned.substr(0, returned.length()-2);
    }
    returned += " int64) int64 {}\n";
    return returned;
}

std::string Translation::Go::funcConstructor(Translation::Go::ptr_t<Parser::FunctionDeclaration> func) {
    return "var " + func->symbol() + " = " + "__func__" + func->symbol() + "__CONSTRUCTOR__()";
}


std::string Translation::Go::parseMain(Translation::Go::ptr_t<Parser::FunctionDeclaration> func) {
    return "\n__MAIN_PARSING_AND_SO_ON__\n";
}


std::string Translation::Go::parseCodeBlock(Translation::Go::Node_ptr node) {
    return std::__cxx11::string();
}

std::string Translation::Go::parseExpr(Translation::Go::Node_ptr node) {
    return std::__cxx11::string();
}

std::string Translation::Go::parseCritical(Translation::Go::Node_ptr critical, std::uint32_t mut_id) {
    return std::__cxx11::string();
}

std::string Translation::Go::parseConcurrent(Translation::Go::Node_ptr concurrent) {
    return std::__cxx11::string();
}

std::string Translation::Go::addIntend(std::string original) {
    std::string intend = "";
    for(std::uint32_t i = 0; i < _intend; ++i) {
        intend += "\t";
    }
    return intend + original;
}

void Translation::Go::incIntend() {
    ++_intend;
}

void Translation::Go::decIntend() {
    if(!_intend) {
        throw std::runtime_error("Intend cannot be lower than 0!");
    }
    --_intend;
}
