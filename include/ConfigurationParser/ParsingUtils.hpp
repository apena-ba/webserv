#ifndef PARSING_UTILS_HPP
#define PARSING_UTILS_HPP

#include <iostream>
#include <vector>
#include <fstream>

class ParsingUtils {
public:
    ParsingUtils();

    ~ParsingUtils();


    static unsigned int strContainsChar(std::string str, char c) {
        unsigned int count = 0;
        for (unsigned int i = 0; i < str.size(); i++) {
            if (str[i] == c) {
                count++;
            }
        }
        return count;
    }

    static bool strContainsSpace(std::string str) {
        for (unsigned int i = 0; i < str.size(); i++) {
            if (std::isspace(str[i]))
                return true;
        }
        return false;
    }

    static bool vectorContainsSpace(std::vector<std::string> vec) {
        for (unsigned int i = 0; i < vec.size(); i++) {
            if (strContainsSpace(vec[i]))
                return true;
        }
        return false;
    }

    static std::vector<std::string> split(std::string str, std::string delimiter) {
        std::vector<std::string> result;
        size_t pos = 0;
        std::string token;
        while ((pos = str.find(delimiter)) != std::string::npos) {
            token = str.substr(0, pos);
            result.push_back(token);
            str.erase(0, pos + delimiter.length());
        }
        result.push_back(str);
        return result;
    }

    static bool strIsDigit(std::string str) {
        for (unsigned int i = 0; i < str.size(); i++) {
            if (!std::isdigit(str[i]))
                return false;
        }
        return true;
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

    static bool checkLimiter(std::vector<std::string> &server,
                             char open_limiter, char close_limiter) {
        int openLimiterNumber = 0;
        int closeLimiterNumber = 0;
        for (unsigned int i = 0; i < server.size(); i++) {
            for (unsigned int j = 0; j < server[i].size(); j++) {
                if (server[i][j] == open_limiter) {
                    openLimiterNumber++;
                } else if (server[i][j] == close_limiter) {
                    closeLimiterNumber++;
                    if (closeLimiterNumber > openLimiterNumber) {
                        throw ErrorParsing("Error: Bad server format: More close limiter than open limiter");
                    }
                }
            }
        }
        if (openLimiterNumber != closeLimiterNumber) {
            throw ErrorParsing("Error: Bad server format: Not same number of open and close limiter");
        }
        return true;
    }

    static std::string fileToString(std::string filePath) {
        std::ifstream file(filePath);
        std::string str((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
        return str;
    }

    static std::vector<std::string> removeEmptyLine(std::vector<std::string> file) {
        std::vector<std::string> result;
        for (unsigned int i = 0; i < file.size(); i++) {
            if (file[i] != "")
                result.push_back(file[i]);
        }
        return result;
    }

    static void removeAllSpace(std::string &str) {
        for (unsigned int i = 0; i < str.size(); i++) {
            if (str[i] == ' ') {
                str.erase(i, 1);
                i--;
            }
        }
    }
};

#endif