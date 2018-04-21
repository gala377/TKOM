//
// Created by igor on 21.04.18.
//

#ifndef TRANSLATOR_SCOPE_H
#define TRANSLATOR_SCOPE_H

#include <string>
#include <bits/unique_ptr.h>

#include "Lexer/Token.h"

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
        Scope(Scope* parent=nullptr);

        Scope& newSubScope();

        void setParent(Scope *parent);
        void addIdentifier(Identifier id);

        Scope& parent() const;
        const Identifier& find(std::string symbol) const;

        bool isDefined(std::string symbol) const;
    private:
        std::unique_ptr<Scope> _parent;
        std::vector<Identifier> _defined_ids;
    };
}

#endif //TRANSLATOR_SCOPE_H
