//
// Created by igor on 21.04.18.
//

#ifndef TRANSLATOR_SCOPE_H
#define TRANSLATOR_SCOPE_H

#include <string>
#include <bits/unique_ptr.h>
#include <optional>

#include "Lexer/Token.h"
#include "Exceptions/Utils.h"

namespace Parser {

    struct Identifier {
        std::string symbol;

        enum Type {
            function, variable
        };
        Type type;
    };

    class Scope {
    public:
        explicit Scope(Scope* parent = nullptr);

        Scope& newSubScope();

        void addIdentifier(Identifier id);

        const Identifier& find(std::string symbol) const;
        bool isDefined(std::string symbol) const;
    private:
        Scope* _parent;
        std::vector<Scope> _children;
        std::vector<Identifier> _defined_ids;
    };
}

#endif //TRANSLATOR_SCOPE_H
