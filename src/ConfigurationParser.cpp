#include "ConfigurationParser.hpp"



bool ConfigurationParser::_checkFile(std::ifstream & file){
    std::string line;
    line = "";
    while (line != "EOF") {
        _checkServer(file);
        
        file >> line;
    }
    return true;
}

bool ConfigurationParser::_strContainsSpace(std::string str){
    for (unsigned int i = 0; i < str.size(); i++){
        if (std::isspace(str[i]))
            return true;
    }
    return false;
}

bool ConfigurationParser::_vectorContainsSpace(std::vector <std::string> vec){
    for (unsigned int i = 0; i < vec.size(); i++){
        if (_strContainsSpace(vec[i]))
            return true;
    }
    return false;
}

std::vector <std::string> ConfigurationParser::_split(std::string str, std::string delimiter){
    std::vector <std::string> result;
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

bool ConfigurationParser::_strIsDigit(std::string str){
    for (unsigned int i = 0; i < str.size(); i++){
        if (!std::isdigit(str[i]))
            return false;
    }
    return true;
}

bool ConfigurationParser::_checkRoutes(std::ifstream & file){
    return true;
}

void _removeSpace(std::string & str){
    std::string result;
    for (unsigned int i = 0; i < str.size(); i++){
        if (!std::isspace(str[i]))
            result += str[i];
    }
    str = result;
}

bool ConfigurationParser::_checkServer(std::ifstream & file){
    std::string line;
    std::vector <std::string> splitted;
    for (unsigned int index = 0; index < 5; index++){
        
        _removeSpace(line);

        file >> line;
        std::cout << line << std::endl;
        splitted = _split(line, ":");
        switch (index){
            case 0:
                if (splitted.size() != 1 || line[line.size() - 1] != '{')
                    {
                        std::cout << line << std::endl;
                        throw BadFile("problem name");
                    }
                break;
            case 1:
                if (splitted[0] != "port" || splitted.size() != 2 ||
                !this->_strIsDigit(splitted[1]) || std::stoi(splitted[1]) < 1024 || std::stoi(splitted[1]) > 65535)
                    {
                        throw BadFile("problem port");
                    }
                break;
            case 2:
                if (splitted[0] != "default_error_page" || splitted.size() != 2)
                    throw BadFile("problem default error page");
                break;
            case 3:
                if (splitted[0] != "client_body_size_max" || splitted.size() != 2 || !this->_strIsDigit(splitted[1]))
                    throw BadFile("problem client body size max");
                break;
            case 4:
                if (splitted[0] != "max_clients" || splitted.size() != 2)
                    throw BadFile("problem max clients");
                break;
            case 5:
                if (splitted[0] != "routes" || splitted.size() != 2 || !this->_checkRoutes(file))
                    throw BadFile("problem routes");
                break;
            case 6:
                if (splitted[0] != "}" || splitted.size() != 2)
                    throw BadFile("problem }");
                break;
            default:
                throw BadFile("Unknown error");
        }
    }
    return true;
}

ConfigurationParser::ConfigurationParser(){}
ConfigurationParser::~ConfigurationParser(){}
void ConfigurationParser::fromFile(std::string path){
    std::ifstream file (path);
    if (file.is_open()){
        _checkFile(file);
    }
    else{
        throw BadFile("Failed to open file");
    }
}