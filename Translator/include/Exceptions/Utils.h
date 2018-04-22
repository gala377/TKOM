//
// Created by igor on 24.03.18.
//

#ifndef TRANSLATOR_UTILS_H
#define TRANSLATOR_UTILS_H

#include <exception>
#include <optional>

template <typename T>
using opt_ref = std::optional<std::reference_wrapper<T>>;

class UnimplementedError : public std::exception {

    virtual const char* what() const noexcept {
        return "Method is unimplemented";
    }
};

#endif //TRANSLATOR_UTILS_H
