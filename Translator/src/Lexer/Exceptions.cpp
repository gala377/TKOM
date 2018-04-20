//
// Created by igor on 14.04.18.
//

#include "Lexer/Exceptions.h"


Error::Error(std::uint32_t line,
             std::uint32_t position,
             std::string message): _line(line),
                                                     _position(position),
                                                     _message(std::move(message)) {}

const char *Error::what() const noexcept {
    return ("Exception in line: " +
           std::to_string(_line) +
           " character: " +
           std::to_string(_position) +
           ": " + _message).c_str();
}



ExpectedError::ExpectedError(std::uint32_t line,
                             std::uint32_t position,
                             std::string expected,
                             std::string got) : Error(line, position) {
    _message += " expected: " + expected + " got: " + got;
}
