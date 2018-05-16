//
// Created by igor on 16.05.18.
//

#ifndef TRANSLATOR_LOGGER_H
#define TRANSLATOR_LOGGER_H

#include <ostream>
#include <type_traits>
#include <iostream>

namespace Logging {

    template <typename T>
    struct is_string_trait {
        static const bool value = false;
    };

    template <>
    struct is_string_trait<char*> {
        static const bool value = true;
    };
    template <>
    struct is_string_trait<const char*> {
        static const bool value = true;
    };
    template <>
    struct is_string_trait<std::string> {
        static const bool value = true;
    };

    template <typename T>
    constexpr bool is_string = is_string_trait<T>::value;


    class Logger {
    public:
        Logger();

        Logger(std::ostream& out, std::string prefix = "");
        Logger(Logger& other);
        Logger(Logger& other, std::string prefix);

        template<typename T>
        Logger& operator<<(T printable) {
            writeToStream(printable);
            _prefix_written = !isEndOfLine<T>()(printable);
            return *this;
        }


        Logger newSubLogger(std::string prefix);

    private:
        bool _is_null_stream;
        std::ostream& _out;

        std::string _prefix;
        bool _prefix_written;

        template <typename T, bool string_trait = is_string<T>>
        class isEndOfLine {
        public:
            bool operator()(T printable) const {
                return false;
            }
        };

        template <typename T>
        class isEndOfLine<T, true> {
        public:
            bool operator()(T printable) const {
                return std::string(printable) == std::string("\n");
            }
        };

        template <typename T>
        void writeToStream(T printable) {
            if(!_prefix_written) {
                _out << _prefix;
                _prefix_written = true;
            }
            _out << printable;
        }
    };

}
#endif //TRANSLATOR_LOGGER_H
