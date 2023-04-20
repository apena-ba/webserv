#ifndef MACRO_HPP
#define MACRO_HPP
#include <iostream>
#include <vector>
class ParsingUtils
{
public:
    ParsingUtils();
    ~ParsingUtils();
    bool isDigit(std::string str);
    bool isSpace(std::string str);
    bool isSpace(std::vector<std::string> vec);
    std::vector<std::string> split(std::string str, std::string delimiter);

    bool strContainsSpace(std::string str)
    {
        for (unsigned int i = 0; i < str.size(); i++)
        {
            if (std::isspace(str[i]))
                return true;
        }
        return false;
    }

    bool vectorContainsSpace(std::vector<std::string> vec)
    {
        for (unsigned int i = 0; i < vec.size(); i++)
        {
            if (_strContainsSpace(vec[i]))
                return true;
        }
        return false;
    }

    std::vector<std::string> split(std::string str, std::string delimiter)
    {
        std::vector<std::string> result;
        size_t pos = 0;
        std::string token;
        while ((pos = str.find(delimiter)) != std::string::npos)
        {
            token = str.substr(0, pos);
            result.push_back(token);
            str.erase(0, pos + delimiter.length());
        }
        result.push_back(str);
        return result;
    }

    bool strIsDigit(std::string str)
    {
        for (unsigned int i = 0; i < str.size(); i++)
        {
            if (!std::isdigit(str[i]))
                return false;
        }
        return true;
    }
};

#endif