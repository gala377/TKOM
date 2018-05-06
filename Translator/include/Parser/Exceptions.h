//
// Created by igor on 07.05.18.
//

#ifndef TRANSLATOR_PARSER_EXCEPTIONS_H
#define TRANSLATOR_PARSER_EXCEPTIONS_H

#include <string>

#include <Lexer/Exceptions.h>


class IdentifierRedeclaration: public Error {
public:
    IdentifierRedeclaration(std::uint32_t line,
                            std::uint32_t position,
                            std::string symbol);
};

class UndefinedIdentifier: public Error {
public:
    UndefinedIdentifier(std::uint32_t line,
                        std::uint32_t position,
                        std::string symbol);
};

#endif //TRANSLATOR_EXCEPTIONS_H
