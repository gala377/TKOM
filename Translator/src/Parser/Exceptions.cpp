//
// Created by igor on 07.05.18.
//

#include <Parser/Exceptions.h>

IdentifierRedeclaration::IdentifierRedeclaration(std::uint32_t line,
                                                 std::uint32_t position,
                                                 std::string symbol): Error(
                                                         line, position) {
    _message += " redeclaration of identifier " + symbol;
}

UndefinedIdentifier::UndefinedIdentifier(std::uint32_t line,
                                         std::uint32_t position,
                                         std::string symbol) : Error(
                                                 line, position) {
    _message += " undefined identifier " + symbol;

}
