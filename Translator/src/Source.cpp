//
// Created by igor on 14.04.18.
//

#include <iostream>

#include "Source.h"
#include "Lexer/Characters.h"


Source::Source(std::string path) {
    _file.open(path);
    if(_file.bad()) {
        throw std::runtime_error("Lexer could not open the file");
    }
    _is_file_opened = true;
}

Source::~Source() {
    if(opened()) {
        closeFile();
    }
}


void Source::closeFile() {
    _is_file_opened = false;
    _file.close();
}


char Source::getChar() {
    if (!_is_file_opened) {
        throw std::runtime_error("File needs to be opened before reading from it");
    }

    char ch;
    _file >> std::noskipws >> ch;

    if(Syntax::isNewLine(ch)) {
        _in_line_position = 0;
        ++_file_line;
    } else {
        ++_in_line_position;
    }

    return ch;
}

char Source::getNextChar() {
    char ch;
    if(_last_read_ch) {
        ch = _last_read_ch;
        _last_read_ch = '\0';
        // std::cout << "Starting from last ch: " << ch << "\n";
    } else {
        ch = getChar();
        // std::cout << "Read new char: " << ch << "\n";
    }
    return ch;
}

char Source::getNextNonBlankChar() {
    // std::cout << "Skipping spaces: ";
    auto ch = getChar();
    while (Syntax::isBlank(ch)) {
        // std::cout << ".";
        ch = getChar();
    }
    // std::cout << "\nRead: " << ch << "\n";
    return ch;
}


void Source::ungetChar(char ch) {
    _last_read_ch = ch;
}


bool Source::eof() const {
    return _file.eof();
}


std::uint32_t Source::line() const {
    return _file_line;
}

std::uint32_t Source::inLinePosition() const {
    return _in_line_position;
}


bool Source::opened() const {
    return _is_file_opened;
}
