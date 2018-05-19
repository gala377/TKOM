//
// Created by igor on 19.05.18.
//

#include <cassert>
#include <string>
#include <iostream>

#include <Parser/Nodes.h>
#include "Translator/Go.h"


// todo return in critical
// todo return in concurrent - error
// todo variable not used - error
// todo add return to every function

std::string Translation::Go::translate(Parser::Tree& abstr_tree) {
    std::cout << "Translating tree to Go:\n";
    abstr_tree.goToRoot();
    auto res = parseDocument(abstr_tree.getCurrent());
    if(!_main_func_present) {
        throw std::runtime_error("Expected main function definition");
    }
    return res;
}



std::string Translation::Go::parseDocument(Translation::Go::Node_ptr node) {
    std::cout << "Parsing document\n";
    std::string document;
    for(const auto& child: node->getChildren()) {
        document += parseFunctionDeclaration(child.get());
    }

    document = documentHeader() + document;
    return document;
}

std::string Translation::Go::documentHeader() {
    std::cout << "Document header\n";
    std::string head = "package main\n\n";
    for(const auto& import: _imports) {
        head += "import \"" + import + "\"\n";
    }
    return head + "\n";
}



std::string Translation::Go::parseFunctionDeclaration(Translation::Go::Node_ptr node) {
    std::cout << "Func declaration\n";
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

    _critical_count = 0;
    _return_at_the_end_of_func = false;

    return header + "{\n" + prelude + body + "}\n" + constructor;
}

void Translation::Go::assertFlagsAreZeroed() {
    assert(!_return_at_the_end_of_func);
    assert(!_critical_count);
}


std::string Translation::Go::functionHeader(ptr_t<Parser::FunctionDeclaration> func) {
    std::cout << "func header\n";
    std::string header = "func __func__" + func->symbol() + "__CONSTRUCTOR__()";
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
    std::cout << "func prelude\n";
    if(_critical_count > 0) {
        _imports.insert("sync");
    }
    auto mutexes = mutexesDeclarations(_critical_count);
    std::string prelude = addIntend("return ");
    return mutexes + prelude;
}


std::string Translation::Go::returnedFunction(ptr_t<Parser::FunctionDeclaration> func) {
    std::cout << "function body\n";
    std::string returned = "func(";
    for(const auto& arg: func->args()) {
        returned += arg + ", ";
    }
    if(returned[returned.length()-2] == ',') {
        returned = returned.substr(0, returned.length()-2);
        returned += " int64";
    }
    returned += ") int64 ";

    incIntend();
    auto body = parseCodeBlock(func);
    decIntend();
    return returned + "{\n" + body + addIntend("}\n");
}

std::string Translation::Go::funcConstructor(ptr_t<Parser::FunctionDeclaration> func) {
    std::cout << "func constructor\n";
    return "var " + func->symbol() + " = " + "__func__" + func->symbol() + "__CONSTRUCTOR__()\n\n";
}

std::string Translation::Go::parseMain(ptr_t<Parser::FunctionDeclaration> func) {
    std::cout << "parsing main\n";
    std::string header = R"(func main() {)";
    header += "\n";

    incIntend();


    std::string body = parseCodeBlock(func);

    std::string mutexes;
    if(_critical_count > 0) {
        _imports.insert("sync");
        mutexes = mutexesDeclarations(_critical_count);
    }


    auto tail = addIntend("}\n\n");
    decIntend();

    _return_at_the_end_of_func = false;
    _critical_count = 0;

    return header + mutexes + body + tail;
}


std::string Translation::Go::mutexesDeclarations(std::uint32_t n) {
    std::cout << "mutexes declaration\n";
    std::string res;
    for(std::uint32_t i = 0; i < n; ++i) {
        std::cout << "Mutex num " << i << " \n";
        std::string line = "__FUNC_MUTEX_ID_" + std::to_string(i);
        line += " := &sync.Mutex{}\n";
        res += addIntend(line);
    }
    return res;
}



std::string Translation::Go::parseCodeBlock(Translation::Go::Node_ptr node) {
    std::cout << "parsing code block\n";
    std::string res;
    for(const auto& child: node->getChildren()) {
        if(auto expr = dynamic_cast<Parser::Expression*>(child.get()); expr != nullptr) {
            res += parseExpr(expr);
        } else if(auto casted = dynamic_cast<Parser::Statement*>(child.get());
                    casted != nullptr) {
            res += parseStatement(casted);
        } else {
            throw std::runtime_error("unexpected node in code block");
        }

    }
    return res;
}

std::string Translation::Go::parseExpr(ptr_t<Parser::Expression> expr) {
    std::cout << "parsing expr\n";
    if(dynamic_cast<Parser::PrintCall*>(expr)) {
        _imports.insert("fmt");
        return addIntend(expr->parse());
    }
    if(auto call = dynamic_cast<Parser::LibraryFunctionCall*>(expr); call != nullptr) {
        _imports.insert(call->library());
    }
    return addIntend(expr->parse());
}

std::string Translation::Go::parseStatement(Translation::Go::ptr_t<Parser::Statement> statement) {
    std::cout << "parsing statement\n";
    if(statement->symbol() == "for") {
        std::string loop = "for ";
        loop += statement->expr()->parse();
        loop += " {\n";
        loop = addIntend(loop);
        incIntend();
        loop += parseCodeBlock(statement);
        decIntend();
        loop += addIntend("}\n\n");
        return loop;
    } else if(statement->symbol() == "return") {
        return addIntend(statement->parse());
    } else if(statement->symbol() == "critical") {
        return parseCritical(statement);
    } else if(statement->symbol() == "concurrent") {
        return parseConcurrent(statement);
    } else if(statement->symbol() == "if") {
        throw std::runtime_error("Implement me! if");
        // todo
    } else if(statement->symbol() == "") {
        std::string res = addIntend("{\n");
        incIntend();
        res += parseCodeBlock(statement);
        decIntend();
        res += addIntend("}\n\n");
        return res;
    }
    throw std::runtime_error("Unexpected symbol of expr: " + statement->symbol());
}


std::string Translation::Go::parseCritical(ptr_t<Parser::Statement> critical) {
    std::cout << "parsing critical\n";
    _imports.insert("sync");
    std::string mutex_name = "__FUNC_MUTEX_ID_" + std::to_string(_critical_count);
    _critical_count++;
    auto res = addIntend(mutex_name);
    res += ".Lock()\n";
    incIntend();
    auto body = parseCodeBlock(critical);
    decIntend();
    return res + body + addIntend(mutex_name) + ".Unlock()\n\n";
}

std::string Translation::Go::parseConcurrent(ptr_t<Parser::Statement> concurrent) {
    _imports.insert("sync");
    std::string wg_name = "__WAIT_GROUP__" + std::to_string(_wait_group_count);
    ++_wait_group_count;
    std::string res = addIntend("var " + wg_name + " sync.WaitGroup\n");

    for(const auto& child: concurrent->getChildren()) {
        res += addIntend(wg_name + ".Add(1)\n");
        res += addIntend("go func() {\n");
        incIntend();
        res += addIntend("defer " + wg_name + ".Done()\n");
        res += parseCodeBlock(child.get());
        decIntend();
        res += addIntend("}()\n");
    }

    res += addIntend(wg_name + ".Wait()\n\n");
    return res;
}



std::string Translation::Go::addIntend(std::string original) {
    std::string intend;
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
