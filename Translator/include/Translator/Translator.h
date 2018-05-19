//
// Created by igor on 19.05.18.
//

#ifndef TRANSLATOR_TRANSLATOR_H
#define TRANSLATOR_TRANSLATOR_H

#include <string>

#include "Parser/Tree.h"

namespace Translation {

    class AbstractBase {
    public:
        virtual std::string translate(Parser::Tree& abstr_tree) = 0;

    };
}


#endif //TRANSLATOR_TRANSLATOR_H
