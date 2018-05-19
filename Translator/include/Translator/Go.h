//
// Created by igor on 19.05.18.
//

#ifndef TRANSLATOR_GO_H
#define TRANSLATOR_GO_H

#include <Parser/Nodes.h>
#include "Translator.h"

namespace Translation {

    class Go: public AbstractBase {
    public:
        using Node_ptr = Parser::Tree::Node*;

        template<typename T>
        using ptr_t = T*;

        std::string translate(Parser::Tree& abstr_tree) override;

    protected:
        // Parsing flags
        bool _add_fmt_import = false;
        bool _add_sync_import = false;


        bool _return_at_the_end_of_func = false;
        std::uint32_t _critical_count = 0;

        bool _main_func_present = false;

        // Parsing function definitions
        std::string parseDocument(Node_ptr node);
        std::string documentHeader();

        std::string parseFunctionDeclaration(Node_ptr node);
        void assertFlagsAreZeroed();
        std::string functionHeader(ptr_t<Parser::FunctionDeclaration> func);
        std::string functionPrelude(ptr_t<Parser::FunctionDeclaration> func);
        std::string returnedFunction(ptr_t<Parser::FunctionDeclaration> func);
        std::string funcConstructor(ptr_t<Parser::FunctionDeclaration> func);

        std::string parseMain(ptr_t<Parser::FunctionDeclaration> func);

        std::string parseCodeBlock(Node_ptr node);
        std::string parseExpr(Node_ptr node);


        std::string parseCritical(Node_ptr critical, std::uint32_t mut_id);
        std::string parseConcurrent(Node_ptr concurrent);

        std::string addIntend(std::string original);

        // Intend stuff
        std::uint32_t _intend = 0;

        void incIntend();
        void decIntend();


    };

}


#endif //TRANSLATOR_GO_H
