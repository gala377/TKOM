#include "Parser/Parser.h"
#include "Parser/Exceptions.h"
#include "Lexer/Token.h"
#include "Lexer/Exceptions.h"
#include "Parser/Nodes.h"


#include <iostream>

using namespace Parser;
using token_id_t = Syntax::Token::Identifier;
using token_type_t = Syntax::Token::Type;

// todo print function call
// todo all functions should return something

Parser::Parser::Parser(Syntax::Lexer& lexer) : _lexer(lexer), _scope(Scope()) {
    auto root = new Document;
    _tree = Tree(root);
};

Tree Parser::Parser::parse() {
    _lexer.newContext(true, true);
    for(auto curr = _lexer.nextToken();
        curr.type() != token_type_t::Eof;
        curr = _lexer.nextToken()) {
        
        if(curr.type() == token_type_t::Comment) {
            continue;
        } else if(curr.identifier() == token_id_t::Function) {
            log("parse() - Parsing new function");
            _tree.getCurrent()->addChild( parseFunction() );
            log("Function parsed");
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
    if(auto curr = _lexer.nextToken(); curr.identifier() != token_id_t::OpenCurlyBracker) {
        throw exception<ExpectedError>("{", curr.symbol());
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
    if (auto curr = _lexer.nextToken(); curr.identifier() == token_id_t::Identifier) {
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
    if(auto curr = _lexer.nextToken(); curr.identifier() != token_id_t::OpenBracket) {
        throw exception<ExpectedError>("(", curr.symbol());
    }
    switch(auto curr = _lexer.nextToken(); curr.identifier()) {
        case token_id_t::CloseBracket:
        break;
        case token_id_t::Identifier:
        args.push_back(curr.symbol());
        for(curr = _lexer.nextToken();
         curr.identifier() != token_id_t::CloseBracket;
         curr = _lexer.nextToken()) {
             if(curr.identifier() != token_id_t::Comma) {
                 throw exception<ExpectedError>(",", curr.symbol());
             }
             if(curr = _lexer.nextToken(); curr.identifier() != token_id_t::Identifier) {
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
         curr.identifier() != token_id_t::CloseCurlyBracket;
         curr = _lexer.nextToken()) {

        if(curr.identifier() == token_id_t::Comment) {
            log("Comment");
            continue;
        } else if(curr.identifier() == token_id_t::Variable) {
            log("Let");
            expressions.emplace_back(parseVariableDeclaration(current_scope));
        } else if(curr.identifier() == token_id_t::Return) {
            log("Return");
            expressions.emplace_back(parseReturn(current_scope));
        } else if(curr.identifier() == token_id_t::Loop) {
            log("Loop");
            expressions.emplace_back(parseLoop(current_scope));
        } else if(curr.identifier() == token_id_t::If) {
            log("If");
            expressions.emplace_back(parseIf(current_scope));
        } else if(curr.identifier() == token_id_t::Concurrent) {
            log("Concurrent");
            expressions.emplace_back(parseConcurrent(current_scope));
        } else if(curr.identifier() == token_id_t::Critical) {
            log("Critical");
            expressions.emplace_back(parseCritical(current_scope));
        } else if(curr.identifier() == token_id_t::OpenCurlyBracker) {
            log("Block");
            expressions.emplace_back(parseCodeBlockStatement(current_scope));
        } else if(curr.identifier() == token_id_t::Print) {
            log("Print");
            expressions.emplace_back(parsePrintCall(current_scope));
        } else if(curr.identifier() == token_id_t::Identifier) {
            log("Identifier");
            //todo make it another function, ones again unget_token is needed for that
            if(current_scope.isDefined(curr.symbol())) {
                if(auto id = current_scope.find(curr.symbol()); id.type == Identifier::Type::function) {
                    auto args = parseFunctionParameters(current_scope);
                    expressions.emplace_back(std::make_shared<FunctionCall>(
                            id.symbol,
                            args));
                } else {
                    //todo func parse_var_identifier
                    if(curr = _lexer.nextToken(); curr.identifier() != token_id_t::Assignment) {
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

FunctionCall::args_t Parser::Parser::parseFunctionParameters(Scope& enveloping_scope) {
    _lexer.newContext(true, true);
    std::vector<std::string> args = {};
    auto curr = _lexer.nextToken();
    if(curr.identifier() != token_id_t::OpenBracket) {
        throw exception<ExpectedError>("(", curr.symbol());
    }
    if(curr = _lexer.nextToken(); curr.identifier() == token_id_t::CloseBracket) {
        return args;
    }
    _lexer.ungetToken(curr);
    while(true) {
        // todo make it children
        args.emplace_back(parseExpression(enveloping_scope)->parse());
        if(curr = _lexer.nextToken(); curr.identifier() == token_id_t::CloseBracket) {
            break;
        }
        if(curr.identifier() != token_id_t::Comma) {
            throw exception<ExpectedError>(",", curr.symbol());
        }
    }

    _lexer.retrieveContext();
    return args;
}


std::shared_ptr<Expression> Parser::Parser::parseVariableDeclaration(Scope& enveloping_scope) {
    _lexer.newContext(true, false);
    auto curr = _lexer.nextToken();

    if(curr.identifier() != token_id_t::Identifier) {
        throw exception<ExpectedError>("identifier", curr.symbol());
    }
    if(enveloping_scope.isDefined(curr.symbol())) {
        throw exception<IdentifierRedeclaration>(curr.symbol());
    }
    auto new_identifier = curr.symbol();
    auto left_side = std::make_shared<VariableDeclaration>(curr.symbol());

    curr = _lexer.nextToken();
    if(curr.identifier() != token_id_t::Assignment) {
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

    log("Left side parsed ", left_side->repr());
    auto curr = _lexer.nextToken();
    if(curr.identifier() == token_id_t::NewLine) {
        right_side = std::make_shared<Empty>();
        log("New line - right side is empty");
    } else if(curr.identifier() == token_id_t::OpenCurlyBracker ||
            curr.identifier() == token_id_t::Comma) {
        _lexer.ungetToken(curr);
        right_side = std::make_shared<Empty>();
    } else if(curr.type() != token_type_t::Operator) {
        throw exception<ExpectedError>("Operator", curr.symbol());
    } else {
        log("Parsing right side");
        if(curr.identifier() == token_id_t::CloseBracket) {
            _lexer.ungetToken(curr);
            right_side = std::make_shared<Empty>();
        } else {
            expr_operator = curr.symbol();
            log("Operator is ", expr_operator);
            if(!isOperatorValidInExpr(curr.identifier())) {
                throw exception<ExpectedError>("+, -, *, /, <, >, ==, !=", expr_operator);
            }

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
    if(curr.identifier() == token_id_t::NewLine) {
        throw exception<ExpectedError>(")", "new line");
    } else if(curr.type() != token_type_t::Operator) {
        throw exception<ExpectedError>("operator", curr.symbol());
    } else if(curr.identifier() == token_id_t::CloseBracket) {
        right_side = std::make_shared<Empty>();
        _lexer.ungetToken(curr);
        log(") encauntered - right side is empty");
    } else {
        expr_operator = curr.symbol();
        // check if symbol is valid in this context
        // + - * /
        log("Operator found: ", expr_operator);
        right_side = parseExpression(enveloping_scope);
        log("Right side parsed ", right_side->repr());
    }

    if(curr = _lexer.nextToken(); curr.identifier() != token_id_t::CloseBracket) {
        throw exception<ExpectedError>(")", curr.symbol());
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
    if(curr.identifier() == token_id_t::OpenBracket) {
        left_side = parseBracketExpression(enveloping_scope);
    } else if(curr.identifier() == token_id_t::ConstExpr) {
        left_side = std::make_shared<ConstExpr>(curr.symbol());
        log("Const expr ", left_side->repr());
    } else if(curr.identifier() == token_id_t::Minus) {
        auto op = curr.symbol();
        left_side = std::make_shared<Expression>(
                std::make_shared<Empty>(),
                parseLeftSideOfExpr(enveloping_scope),
                op);
    } else if(curr.identifier() == token_id_t::Identifier) {
        if(enveloping_scope.isDefined(curr.symbol())) {
            if(auto id = enveloping_scope.find(curr.symbol()); id.type == Identifier::Type::function) {
                auto args = parseFunctionParameters(enveloping_scope);
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

bool Parser::Parser::isOperatorValidInExpr(token_id_t op) const {
    return (bool)_EXPR_OPERATORS.count(op);
}


std::shared_ptr<Statement> Parser::Parser::parseCodeBlockStatement(Scope& enveloping_scope) {
    auto curr_scope = enveloping_scope.newSubScope();
    return std::make_shared<BlockStatement>("", parseCodeBlock(curr_scope));
}

std::shared_ptr<Statement> Parser::Parser::parseReturn(Scope& enveloping_scope) {
    return std::make_shared<Statement>("return", parseExpression(enveloping_scope));
}

std::shared_ptr<Statement> Parser::Parser::parseLoop(Scope& enveloping_scope) {
    auto expr = parseExpression(enveloping_scope);
    auto curr_scope = enveloping_scope.newSubScope();

    if(auto curr = _lexer.nextToken(); curr.identifier() != token_id_t::OpenCurlyBracker) {
        throw exception<ExpectedError>("{", curr.symbol());
    }
    auto block = parseCodeBlock(curr_scope);
    return std::make_shared<BlockStatement>(
            "for",
            expr,
            block);
}

std::shared_ptr<Statement> Parser::Parser::parseIf(Scope& enveloping_scope) {
    auto expr = parseExpression(enveloping_scope);

    auto curr_scope = enveloping_scope.newSubScope();

    if(auto curr = _lexer.nextToken(); curr.identifier() != token_id_t::OpenCurlyBracker) {
        throw exception<ExpectedError>("{", curr.symbol());
    }
    auto block = parseCodeBlock(curr_scope);

    auto if_statement = std::make_shared<IfStatement>(
            expr,
            block);

    if(auto curr = _lexer.nextToken(); curr.identifier() != token_id_t::Else) {
        _lexer.ungetToken(curr);
        return if_statement;
    }

    auto else_scope = enveloping_scope.newSubScope();
    if_statement->setElse(parseElse(else_scope));
    return if_statement;
}

std::shared_ptr<ElseStatement> Parser::Parser::parseElse(Scope& enveloping_scope) {
    auto curr = _lexer.nextToken();
    if(curr.identifier() == token_id_t::If) {
        return std::make_shared<ElseStatement>(
                std::dynamic_pointer_cast<BlockStatement>(
                        parseIf(enveloping_scope)));
    }
    if(curr.identifier() == token_id_t::OpenCurlyBracker) {
        return std::make_shared<ElseStatement>(
                std::dynamic_pointer_cast<BlockStatement>(
                    parseCodeBlockStatement(enveloping_scope)));
    }
    throw exception<ExpectedError>("if or '{'", curr.symbol());
}

std::shared_ptr<Statement> Parser::Parser::parseCritical(Scope& enveloping_scope) {
    auto curr_scope = enveloping_scope.newSubScope();

    if(auto curr = _lexer.nextToken(); curr.identifier() != token_id_t::OpenCurlyBracker) {
        throw exception<ExpectedError>("{", curr.symbol());
    }

    return std::make_shared<BlockStatement>(
            "critical_IN_PROGRESS",
            parseCodeBlock(curr_scope));
}

std::shared_ptr<Statement> Parser::Parser::parseConcurrent(Scope& enveloping_scope) {
    auto curr_scope = enveloping_scope.newSubScope();

    if(auto curr = _lexer.nextToken(); curr.identifier() != token_id_t::OpenCurlyBracker) {
        throw exception<ExpectedError>("{", curr.symbol());
    }

    return std::make_shared<BlockStatement>(
            "concurrent_IN_PROGRESS",
            parseCodeBlock(curr_scope));
}


std::shared_ptr<Expression> Parser::Parser::parsePrintCall(Scope& enveloping_scope) {
    log("in print call");
    if(auto curr = _lexer.nextToken(); curr.identifier() != token_id_t::OpenBracket) {
        log("Wrong symbol");
        throw exception<ExpectedError>("(", curr.symbol());
    }
    log("Parsing parameters");
    auto pars = parsePrintParameters(enveloping_scope);

    _lexer.newContext(true, false);
    if(auto curr = _lexer.nextToken(); curr.identifier() != token_id_t::NewLine) {
        throw exception<ExpectedError>("New line", curr.symbol());
    }
    _lexer.retrieveContext();
    return std::make_shared<PrintCall>(pars);
}

PrintCall::args_t Parser::Parser::parsePrintParameters(Scope& enveloping_scope) {
    _lexer.newContext(true, true);
    std::vector<std::string> args = {};

    log("Peeking for ) sign");
    auto curr = _lexer.nextToken();
    log("Got");
    if(curr.identifier() == token_id_t::CloseBracket) {
        return args;
    }
    log("Starting to parse print parameters");
    _lexer.ungetToken(curr);
    while(true) {
        curr = _lexer.nextToken();
        if(curr.type() == token_type_t::String) {
            args.push_back(curr.symbol());
        } else {
            _lexer.ungetToken(curr);
            args.emplace_back(parseExpression(enveloping_scope)->parse());
        }
        if(curr = _lexer.nextToken(); curr.identifier() == token_id_t::CloseBracket) {
            break;
        }
        if(curr.identifier() != token_id_t::Comma) {
            throw exception<ExpectedError>(",", curr.symbol());
        }
    }
    _lexer.retrieveContext();
    return args;
}
