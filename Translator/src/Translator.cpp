
#include <iostream>
#include <fstream>

#include "Source.h"
#include "Lexer/Lexer.h"
#include "Lexer/Token.h"
#include "Parser/Parser.h"
#include "Logger/Logger.h"
#include "Translator/Go.h"

int main(int argv, char* argc[]) {
    
    //
    // args
    //
    
    if(argv != 3) {
        std::cout << "Expected input_file_name output_file_name!\n";
        std::cout << "Aborted\n";
        return 1;
    }
    std::string input = argc[1];
    std::string output = argc[2];

    //
    // Loggers
    //

    std::ofstream lexer_log_file;
    std::ofstream parser_log_file;

    lexer_log_file.open("output/logs/lexer_log");
    parser_log_file.open("output/logs/parser_log");

    Logging::Logger lexer_log(lexer_log_file);
    Logging::Logger parser_log(parser_log_file);

    //
    // Translation
    //

    Source src(input);
    Syntax::Lexer lex(src, lexer_log);
    Parser::Parser pars(lex, parser_log);
    auto tree = pars.parse();
    Translation::Go translator; 
    std::string result = translator.translate(tree);

    //
    // Save
    //

    std::ofstream output_file;
    output_file.open("output/" + output + ".go");
    output_file << result;

    //
    // cleanup
    //

    output_file.close();
    lexer_log_file.close();
    parser_log_file.close();

    //
    // Epiloque
    //

    std::cout << "No errors is succcess...\n";
    return 0;
}