add_library(Translator
        Lexer/Lexer.cpp
        Lexer/Token.cpp
        Lexer/Characters.cpp
        Parser/Parser.cpp
        Parser/Tree.cpp 
        Parser/Nodes.cpp
        Source.cpp
        Lexer/Exceptions.cpp 
        Parser/Scope.cpp
        Parser/Exceptions.cpp 
        Logger/Logger.cpp 
        Translator/Go.cpp)

macro(tkom_target target_name target_files)
        message("target name = ${target_name}")
        message("Sources are = ${target_files}")
        add_executable(${target_name} ${target_files})
        target_link_libraries(${target_name} Translator)

        set_target_properties( ${target_name}
                PROPERTIES
                ARCHIVE_OUTPUT_DIRECTORY_DEBUG "debug/lib"
                LIBRARY_OUTPUT_DIRECTORY_DEBUG "debug/lib"
                RUNTIME_OUTPUT_DIRECTORY_DEBUG "debug/bin"
                ARCHIVE_OUTPUT_DIRECTORY_RELEASE "release/lib"
                LIBRARY_OUTPUT_DIRECTORY_RELEASE "release/lib"
                RUNTIME_OUTPUT_DIRECTORY_RELEASE "release/bin"
        ) 
endmacro(tkom_target)

tkom_target(conc_to_go Translator.cpp)
tkom_target(debug main.cpp)