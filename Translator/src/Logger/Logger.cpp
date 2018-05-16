//
// Created by igor on 16.05.18.
//

#include <string>
#include <Logger/Logger.h>
#include <iostream>

using namespace Logging;

Logger::Logger(): _out(std::cout), _is_null_stream(true) {}

Logger::Logger(std::ostream& out, std::string prefix): _out(out),
                                                       _prefix(std::move(prefix)),
                                                       _is_null_stream(false),
                                                       _prefix_written(false) { }

Logger::Logger(Logger& other): _out(other._out),
                               _is_null_stream(other._is_null_stream),
                               _prefix(other._prefix),
                               _prefix_written(false) {}


Logger::Logger(Logger& other, std::string prefix): _out(other._out),
                                                   _is_null_stream(other._is_null_stream),
                                                   _prefix(std::move(prefix)),
                                                   _prefix_written(false) { }

Logger Logger::newSubLogger(std::string prefix)  {
    prefix = prefix.empty() ? _prefix : prefix;
    return Logger(this->_out, std::move(prefix));
}

