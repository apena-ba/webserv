#ifndef PARSING_UTILS_HPP
#define PARSING_UTILS_HPP

#include <iostream>
#include <vector>
#include <fstream>

class ParsingUtils {
public:
    ParsingUtils();

    ~ParsingUtils();

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
};

#endif