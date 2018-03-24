//
// Created by igor on 24.03.18.
//

#ifndef TRANSLATOR_LEXER_H
#define TRANSLATOR_LEXER_H

#include <string>
#include <fstream>

namespace Syntax {

    class Lexer {
    public:
        void loadFile(std::string path);

    protected:
        std::fstream file;
    };

}
#endif //TRANSLATOR_LEXER_H