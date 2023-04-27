#ifndef PARSING_UTILS_HPP
#define PARSING_UTILS_HPP

#include <iostream>
#include <vector>
#include <fstream>

class ParsingUtils {
public:
    ParsingUtils() {}

    ~ParsingUtils() {}

    static std::string removeIsSpace(std::string str) {
        std::string ret;
        for (unsigned int i = 0; i < str.size(); i++) {
            if (!(std::isspace(str[i])) && str[i] != '\t' && str[i] != '\n') {
                ret += str[i];
            }
        }
        return ret;
    }

    static int strToPositiveInteger(const std::string &value) {
        int num;
        try {
            num = std::stoi(value);
            if (num < 0) {
                throw ErrorParsing("Error: Negative value for field ");
            }
            return num;
        } catch (std::exception &e) {
            throw ErrorParsing("Error: Field is not a positive integer");
        }
    }

    static std::vector<std::string> split(std::string str, const std::string &delimiter) {
        std::vector<std::string> result;
        size_t pos = 0;
        std::string token;
        while ((pos = str.find(delimiter)) != std::string::npos) {
            token = str.substr(0, pos);
            result.push_back(token);
            str.erase(0, pos + delimiter.length());
        }
        if (str.empty() == false) { result.push_back(str); }
        return result;
    }

    static bool checkLimiter(std::string str) {
        int openLimiterNumber = 0;
        int closeLimiterNumber = 0;

        for (unsigned int i = 0; i < str.size(); i++) {
            if (str[i] == '{') {
                openLimiterNumber++;
            } else if (str[i] == '}') {
                closeLimiterNumber++;
                if (closeLimiterNumber > openLimiterNumber) {
                    throw ErrorParsing("Error: Bad server format: More close limiter than open limiter");
                }
            }
        }
        return true;
    }

    static std::string fileToString(std::string filePath) {
        std::ifstream file(filePath);
        std::string str((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
        return str;
    }

    static void removeAllSpace(std::string &str) {
        for (unsigned int i = 0; i < str.size(); i++) {
            if (str[i] == ' ') {
                str.erase(i, 1);
                i--;
            }
        }
    }

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