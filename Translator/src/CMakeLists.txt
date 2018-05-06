add_executable(Translator
        main.cpp
        Lexer/Lexer.cpp
        Lexer/Token.cpp
        Lexer/Characters.cpp
        Parser/Parser.cpp
        Parser/Tree.cpp Parser/Nodes.cpp ../include/Source.h Source.cpp ../include/Lexer/Exceptions.h Lexer/Exceptions.cpp ../include/Parser/Scope.h Parser/Scope.cpp ../include/Parser/Exceptions.h Parser/Exceptions.cpp)