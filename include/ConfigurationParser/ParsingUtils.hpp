#ifndef PARSING_UTILS_HPP
#define PARSING_UTILS_HPP

#include <iostream>
#include <vector>
#include <fstream>

class ParsingUtils {
public:
    ParsingUtils() {}

    ~ParsingUtils() {}

    static bool checkDoubleValue(std::vector<std::string> vector) {
        for (unsigned int i = 0; i < vector.size(); i++) {
            for (unsigned int j = i + 1; j < vector.size(); j++) {
                if (vector[i] == vector[j]) {
                    return true;
                }
            }
        }
        return false;
    }

    static std::string toUpper(std::string str) {
        for (unsigned int i = 0; i < str.size(); i++) {
            str[i] = std::toupper(str[i]);
        }
        return str;
    }

    static bool isNumber(std::string str) {
        for (unsigned int i = 0; i < str.size(); i++) {
            if (!std::isdigit(str[i])) {
                return false;
            }
        }
        return true;
    }

    static bool betteratoi(const char *str, int &num) {
        long long int tmp;
        if (!isNumber(str)) {
            return false;
        }
        if (std::strlen(str) > 10) {
            return false;
        }
        tmp = 0;
        for (uint32_t i = 0; str[i]; i++) {
            tmp *= 10;
            tmp += str[i] - '0';
            if (tmp > (long long int) std::numeric_limits<int>::max()) {
                return false;
            }
        }
        num = static_cast<int>(tmp);
        return true;
    }

    static std::vector<std::string> toUpperVector(std::vector<std::string> vec) {
        for (unsigned int i = 0; i < vec.size(); i++) {
            vec[i] = toUpper(vec[i]);
        }
        return vec;
    }

    static std::string removeIsSpace(std::string str) {
        std::string ret;
        for (unsigned int i = 0; i < str.size(); i++) {
            if (!(std::isspace(str[i])) && str[i] != '\t' && str[i] != '\n') {
                ret += str[i];
            }
        }
        return ret;
    }

    static std::vector<std::string> split(std::string str, const std::string &delimiter) {
        std::vector<std::string> result;
        size_t pos;
        std::string token;
        while ((pos = str.find(delimiter)) != std::string::npos) {
            token = str.substr(0, pos);
            result.push_back(token);
            str.erase(0, pos + delimiter.length());
        }
        if (!str.empty()) { result.push_back(str); }
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

    static std::string fileToString(const std::string &filePath) {
        std::ifstream file(filePath);
        std::string str((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
        return str;
    }

    static bool checkFileExtension(const std::string &filePath, const std::string &extension) {
        std::string fileExtension = filePath.substr(filePath.find_last_of(".") + 1);
        if (fileExtension != extension) {
            throw ErrorParsing("Error: Bad file extension");
        }
        return true;
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