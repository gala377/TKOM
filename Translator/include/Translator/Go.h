//
// Created by igor on 19.05.18.
//

#ifndef TRANSLATOR_GO_H
#define TRANSLATOR_GO_H

#include <map>
#include <set>

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
        std::uint32_t _wait_group_count = 0;

        bool _main_func_present = false;

        const std::map<std::string, std::string> _LIBRARY_FUNCTIONS = {
                {"Now", "time"},
                {"Sleep", "time"},
        };
        std::set<std::string> _imports;


        // Parsing function definitions
        std::string parseDocument(Node_ptr node);
        std::string documentHeader();

        std::string parseFunctionDeclaration(Node_ptr node);
        void assertFlagsAreZeroed();

        std::string functionHeader(ptr_t<Parser::FunctionDeclaration> func);
        std::string mutexesDeclarations(std::uint32_t n);

        std::string functionPrelude(ptr_t<Parser::FunctionDeclaration> func);
        std::string returnedFunction(ptr_t<Parser::FunctionDeclaration> func);
        std::string funcConstructor(ptr_t<Parser::FunctionDeclaration> func);

        std::string parseMain(ptr_t<Parser::FunctionDeclaration> func);


        std::string parseCodeBlock(Node_ptr node);
        std::string parseExpr(ptr_t<Parser::Expression> expr);
        std::string parseStatement(ptr_t<Parser::Statement> statement);

        std::string parseCritical(ptr_t<Parser::Statement> critical);
        std::string parseConcurrent(ptr_t<Parser::Statement> concurrent);

        std::string parseIf(ptr_t<Parser::Statement> if_node);

        std::string addIntend(std::string original);

        // Intend stuff
        std::uint32_t _intend = 0;

        void incIntend();
        void decIntend();


    };

}


#endif //TRANSLATOR_GO_H
