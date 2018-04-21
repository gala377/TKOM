//
// Created by igor on 14.04.18.
//

#ifndef TRANSLATOR_EXCEPTIONS_H
#define TRANSLATOR_EXCEPTIONS_H

#include <exception>
#include <cstdint>
#include <string>

class Error : public std::exception {
public:
    Error() = default;
    Error(std::uint32_t line, std::uint32_t position, std::string message="Syntax error!");

    virtual const char* what() const noexcept;

protected:
    std::uint32_t _line;
    std::uint32_t _position;

    std::string _message;
};

class ExpectedError : public Error {
public:
    ExpectedError(std::uint32_t line,
                  std::uint32_t position,
                  std::string expected,
                  std::string got);
};

class UnexpectedError : public Error {
public:
    UnexpectedError(std::uint32_t line,
                    std::uint32_t position,
                    std::string symbol);
};

#endif //TRANSLATOR_EXCEPTIONS_H
