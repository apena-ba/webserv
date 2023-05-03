#ifndef PARSING_UTILS_HPP
#define PARSING_UTILS_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include "macro.hpp"

class ParsingUtils {
public:
    ParsingUtils();

    ~ParsingUtils();

    static  bool                checkDoubleValue(VECTOR_STRING vector);

    static  STRING              toUpper(STRING str);

    static  bool                isNumber(STRING str);

    static  bool                betteratoi(const char *str, unsigned int &num);

    static  VECTOR_STRING       toUpperVector(VECTOR_STRING vec);

    static  STRING              removeIsSpace(STRING str);

    static  VECTOR_STRING       split(STRING str, const STRING &delimiter);

    static  bool                checkLimiter(STRING str);

    static  STRING              fileToString(const STRING &filePath);

    class ErrorParsing : public std::exception {
    private:
        const char *_msg;
    public:
        ErrorParsing(const char *msg) : _msg(msg) {};

        virtual const char *what() const throw() {
            return (this->_msg);
        };
    };
};

#endif
