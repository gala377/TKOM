#include "Parser/Parser.h"
#include "Parser/Exceptions.h"
#include "Lexer/Token.h"
#include "Lexer/Exceptions.h"
#include "Parser/Nodes.h"


#include <iostream>

using namespace Parser;

// TODO UgetToken needed for lexer
// we need it for cases like ( expr ) or keyword expr { } or func(arg1, arg1 + arg2)
//                                  ^                 ^               ^
// as they end expression but will be needed higher

// todo print function call
// todo all functions should return something
// todo expressions in function calls
// todo constexpr in function calls

Parser::Parser::Parser(Syntax::Lexer& lexer) : _lexer(lexer), _scope(Scope()) {
    auto root = new Document;
    _tree = Tree(root);
};

Tree Parser::Parser::parse() {
    _lexer.newContext(true, true);
    for(auto curr = _lexer.nextToken();
        curr.type() != Syntax::Token::Type::Eof;
        curr = _lexer.nextToken()) {
        
        if(curr.type() == Syntax::Token::Type::Comment) {
            continue;
        } else if(curr.identifier() == Syntax::Token::Identifier::Function) {
            _tree.getCurrent()->addChild( parseFunction() );
        } else {
            throw exception<ExpectedError>("Comment or Function", curr.symbol());
        }
        log("Waiting for nil");
    }
    log("Returning tree");
    _lexer.retrieveContext();
    return std::move(_tree);
}


std::shared_ptr<Tree::Node> Parser::Parser::parseFunction() {
    auto function = parseFunctionDeclaration();
    auto function_scope = _scope.newSubScope();
    for(auto arg: function->args()) {
        function_scope.addIdentifier({arg, Identifier::variable});
    }
    for(auto child: parseCodeBlock(function_scope)) {
        function->addChild(child);
    }
    log("Returning function");
    return function;
}

std::shared_ptr<FunctionDeclaration> Parser::Parser::parseFunctionDeclaration() {
    auto identifier = parseFunctionIdentifier();
    auto args = parseFunctionArguments();
    return std::shared_ptr<FunctionDeclaration>(
            new FunctionDeclaration(identifier, args));
}

std::string Parser::Parser::parseFunctionIdentifier() {
    _lexer.newContext(true, false);
    if (auto curr = _lexer.nextToken(); curr.identifier() == Syntax::Token::Identifier::Identifier) {
        if(_scope.isDefined(curr.symbol())) {
            throw exception<IdentifierRedeclaration>(curr.symbol());
        }
        _scope.addIdentifier({curr.symbol(), Identifier::function});
        _lexer.retrieveContext();
        return curr.symbol();
    } else {
        throw exception<ExpectedError>("Function name", curr.symbol());
    }
}

std::vector<std::string> Parser::Parser::parseFunctionArguments() {
    _lexer.newContext(true, true);
    std::vector<std::string> args = {};
    if(auto curr = _lexer.nextToken(); curr.identifier() != Syntax::Token::Identifier::OpenBracket) {
        throw exception<ExpectedError>("(", curr.symbol());
    }
    switch(auto curr = _lexer.nextToken(); curr.identifier()) {
        case Syntax::Token::Identifier::CloseBracket:
        break;
        case Syntax::Token::Identifier::Identifier:
        args.push_back(curr.symbol());
        for(curr = _lexer.nextToken();
         curr.identifier() != Syntax::Token::Identifier::CloseBracket;
         curr = _lexer.nextToken()) {
             if(curr.identifier() != Syntax::Token::Identifier::Comma) {
                 throw exception<ExpectedError>(",", curr.symbol());
             }
             if(curr = _lexer.nextToken(); curr.identifier() != Syntax::Token::Identifier::Identifier) {
                 throw exception<ExpectedError>("identifier", curr.symbol());
             }
             args.push_back(curr.symbol());
         }
        break; 
        default:
            throw exception<ExpectedError>("identifier or )", curr.symbol());
    }
    _lexer.retrieveContext();
    return args;
}


std::vector<std::shared_ptr<Tree::Node>> Parser::Parser::parseCodeBlock(Scope& enveloping_scope) {
    _lexer.newContext(true, true);

    std::vector<std::shared_ptr<Tree::Node>> expressions;
    auto current_scope = enveloping_scope.newSubScope();

    log("Parsing code block");
    for (auto curr = _lexer.nextToken();
         curr.identifier() != Syntax::Token::Identifier::CloseCurlyBracket;
         curr = _lexer.nextToken()) {

        if(curr.identifier() == Syntax::Token::Identifier::Comment) {
            log("Comment");
            continue;
        } else if(curr.identifier() == Syntax::Token::Identifier::Variable) {
            log("Let");
            expressions.emplace_back(parseVariableDeclaration(current_scope));
        } else if(curr.identifier() == Syntax::Token::Identifier::Return) {
            log("Return");
            expressions.emplace_back(parseReturn(current_scope));
        } else if(curr.identifier() == Syntax::Token::Identifier::Loop) {
            log("Loop");
            expressions.emplace_back(parseLoop(current_scope));
        } else if(curr.identifier() == Syntax::Token::Identifier::If) {
            log("If");
            expressions.emplace_back(parseIf(current_scope));
        } else if(curr.identifier() == Syntax::Token::Identifier::Concurrent) {
            log("Concurrent");
            expressions.emplace_back(parseConcurrent(current_scope));
        } else if(curr.identifier() == Syntax::Token::Identifier::Critical) {
            log("Critical");
            expressions.emplace_back(parseCritical(current_scope));
        } else if(curr.identifier() == Syntax::Token::Identifier::Identifier) {
            log("Identifier");
            //todo make it another function, ones again unget_token is needed for that
            if(current_scope.isDefined(curr.symbol())) {
                if(auto id = current_scope.find(curr.symbol()); id.type == Identifier::Type::function) {
                    auto args = parseFunctionArguments();
                    for(auto arg: args) {
                        if(!current_scope.isDefined(arg)) {
                            throw exception<UndefinedIdentifier>(arg);
                        }
                    }
                    expressions.emplace_back(std::make_shared<FunctionCall>(
                            id.symbol,
                            args));
                } else {
                    //todo func parse_var_identifier
                    if(curr = _lexer.nextToken(); curr.identifier() != Syntax::Token::Identifier::Assignment) {
                        throw exception<ExpectedError>("=", curr.symbol());
                    }
                    expressions.emplace_back(std::make_shared<Assignment>(
                            std::make_shared<VariableCall>(id.symbol),
                            parseExpression(current_scope)));
                }
            } else {
                throw exception<UndefinedIdentifier>(curr.symbol());
            }

        }
    }
    log("Code block parsed");
    _lexer.retrieveContext();
    return expressions;
}


std::shared_ptr<Expression> Parser::Parser::parseFunctionCall(Scope& enveloping_scope) {
    return std::shared_ptr<Expression>();
}

std::vector<std::string> Parser::Parser::parseFunctionParameters(Scope& enveloping_scope) {
    return std::vector<std::string>();
}


std::shared_ptr<Expression> Parser::Parser::parseVariableDeclaration(Scope& enveloping_scope) {
    _lexer.newContext(true, false);
    auto curr = _lexer.nextToken();

    if(curr.identifier() != Syntax::Token::Identifier::Identifier) {
        throw exception<ExpectedError>("identifier", curr.symbol());
    }
    if(enveloping_scope.isDefined(curr.symbol())) {
        throw exception<IdentifierRedeclaration>(curr.symbol());
    }
    auto new_identifier = curr.symbol();
    auto left_side = std::make_shared<VariableDeclaration>(curr.symbol());

    curr = _lexer.nextToken();
    if(curr.identifier() != Syntax::Token::Identifier::Assignment) {
        throw exception<ExpectedError>("=", curr.symbol());
    }

    auto right_side = parseExpression(enveloping_scope);
    enveloping_scope.addIdentifier({new_identifier, Identifier::variable});
    _lexer.retrieveContext();
    return std::make_shared<Assignment>(left_side, right_side);
}

std::shared_ptr<Expression> Parser::Parser::parseExpression(Scope& enveloping_scope) {
    _lexer.newContext(true, false);
    std::shared_ptr<Expression> left_side, right_side;
    std::string expr_operator = "";

    log("Parsing expr");
    left_side = parseLeftSideOfExpr(enveloping_scope);
    // todo no -2314 for now supported
    // let a = -32432 <- wont be supported for now
    log("Left side parsed ", left_side->repr());
    auto curr = _lexer.nextToken();
    if(curr.identifier() == Syntax::Token::Identifier::NewLine) {
        right_side = std::make_shared<Empty>();
        log("New line - right side is empty");
    } else if(curr.identifier() == Syntax::Token::Identifier::OpenCurlyBracker) {
        // TODO the same case as ( ) - ugly but works
        right_side = std::make_shared<Empty>();
        log("{ sign - right side is empty");
    } else if(curr.type() != Syntax::Token::Type::Operator) {
        throw exception<ExpectedError>("Operator", curr.symbol());
    } else {
        log("Parsing right side");
        // TODO ugly but works for now
        if(curr.identifier() == Syntax::Token::Identifier::CloseBracket) {
            right_side = std::make_shared<Empty>();
        } else {
            expr_operator = curr.symbol();
            log("Operator is ", expr_operator);
            // TODO check if symbol is valid in this context
            // + - * /
            right_side = parseExpression(enveloping_scope);
            log("Right side parsed ", right_side->repr());
        }
    }

    log("Returning expr");
    _lexer.retrieveContext();
    return std::make_shared<Expression>(left_side, right_side, expr_operator);
}

std::shared_ptr<Expression> Parser::Parser::parseBracketExpression(Scope& enveloping_scope) {
    _lexer.newContext(true, false);

    std::shared_ptr<Expression> left_side, right_side;
    std::string expr_operator = "";

    log("Parsing in bracket expr");

    left_side = parseLeftSideOfExpr(enveloping_scope);
    log("Left side parsed ", left_side->repr());
    auto curr = _lexer.nextToken();
    if(curr.identifier() == Syntax::Token::Identifier::NewLine) {
        throw exception<ExpectedError>(")", "new line");
    } else if(curr.type() != Syntax::Token::Type::Operator) {
        throw exception<ExpectedError>("operator", curr.symbol());
    } else if(curr.identifier() == Syntax::Token::Identifier::CloseBracket) {
        right_side = std::make_shared<Empty>();
        log(") encauntered - right side is empty");
    } else {
        expr_operator = curr.symbol();
        // check if symbol is valid in this context
        // + - * /
        log("Operator found: ", expr_operator);
        right_side = parseExpression(enveloping_scope);
        log("Right side parsed ", right_side->repr());
    }

    _lexer.retrieveContext();
    return std::make_shared<InBracketExpr>(
            std::make_shared<Expression>(left_side, right_side, expr_operator));
}


std::shared_ptr<Expression> Parser::Parser::parseLeftSideOfExpr(Scope& enveloping_scope) {
    _lexer.newContext(true, false);
    std::shared_ptr<Expression> left_side;

    log("Parsing left side of the expr");
    auto curr = _lexer.nextToken();
    if(curr.identifier() == Syntax::Token::Identifier::OpenBracket) {
        left_side = parseBracketExpression(enveloping_scope);
    } else if(curr.identifier() == Syntax::Token::Identifier::ConstExpr) {
        left_side = std::make_shared<ConstExpr>(curr.symbol());
        log("Const expr ", left_side->repr());
    } else if(curr.identifier() == Syntax::Token::Identifier::Identifier) {
        if(enveloping_scope.isDefined(curr.symbol())) {
            if(auto id = enveloping_scope.find(curr.symbol()); id.type == Identifier::Type::function) {
                auto args = parseFunctionArguments();
                for(auto arg: args) {
                    if(!enveloping_scope.isDefined(arg)) {
                        throw exception<UndefinedIdentifier>(arg);
                    }
                }
                left_side = std::make_shared<FunctionCall>(id.symbol, args);
            } else {
                left_side = std::make_shared<VariableCall>(curr.symbol());
            }
        } else {
            throw exception<UndefinedIdentifier>(curr.symbol());
        }
    } else {
        throw exception<UnexpectedError>(curr.symbol());
    }

    _lexer.retrieveContext();
    return left_side;
}


std::shared_ptr<Statement> Parser::Parser::parseReturn(Scope& enveloping_scope) {
    return std::make_shared<Statement>("return", parseExpression(enveloping_scope));
}

std::shared_ptr<Statement> Parser::Parser::parseLoop(Scope& enveloping_scope) {
    auto expr = parseExpression(enveloping_scope);
    auto curr_scope = enveloping_scope.newSubScope();
    auto block = parseCodeBlock(curr_scope);
    return std::make_shared<BlockStatement>(
            "for",
            expr,
            block);
}

std::shared_ptr<Statement> Parser::Parser::parseIf(Scope& enveloping_scope) {
    auto expr = parseExpression(enveloping_scope);
    auto curr_scope = enveloping_scope.newSubScope();
    auto block = parseCodeBlock(curr_scope);
    return std::make_shared<BlockStatement>(
            "if",
            expr,
            block);
}

std::shared_ptr<Statement> Parser::Parser::parseCritical(Scope& enveloping_scope) {
    auto curr_scope = enveloping_scope.newSubScope();
    return std::make_shared<BlockStatement>(
            "critical_IN_PROGRESS",
            parseCodeBlock(curr_scope));
}

std::shared_ptr<Statement> Parser::Parser::parseConcurrent(Scope& enveloping_scope) {
    auto curr_scope = enveloping_scope.newSubScope();
    return std::make_shared<BlockStatement>(
            "concurrent_IN_PROGRESS",
            parseCodeBlock(curr_scope));
}
