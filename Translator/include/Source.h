//
// Created by igor on 14.04.18.
//

#ifndef TRANSLATOR_SOURCE_H
#define TRANSLATOR_SOURCE_H


#include <string>
#include <fstream>

class Source {
public:
    Source(std::string path);
    ~Source();

    void closeFile();

    char getChar();
    char getNextChar();
    char getNextNonBlankChar();

    void ungetChar(char ch);

    std::uint32_t line() const;
    std::uint32_t inLinePosition() const;

    bool eof() const;
    bool opened() const;
private:
    // File information
    std::fstream _file;
    bool _is_file_opened = false;

    // Reading information
    std::uint32_t _file_line = 0;
    std::uint32_t _in_line_position = 0;
    char _last_read_ch = '\0';
};

#endif //TRANSLATOR_SOURCE_H
