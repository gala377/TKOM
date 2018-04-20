//
// Created by igor on 24.03.18.
//

#ifndef TRANSLATOR_UTILS_H
#define TRANSLATOR_UTILS_H

#include <exception>

class UnimplementedError : public std::exception {

    virtual const char* what() const noexcept {
        return "Method is unimplemented";
    }
};

#endif //TRANSLATOR_UTILS_H
