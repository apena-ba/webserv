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

bool ConfigurationParser::_checkServer(std::ifstream & file){
    std::string line;
    std::vector <std::string> splitted;
    for (unsigned int index = 0; index < 5; index++){
        file >> line;
        splitted = _split(line, ":");
        if (splitted.size() != 2 || this->_vectorContainsSpace(splitted))
        {throw BadFile();}
        switch (index){
            case 0:
                if (splitted[0] != "port" || !this->_strIsDigit(splitted[1]) || std::stoi(splitted[1]) < 1024 || std::stoi(splitted[1]) > 65535)
                    throw BadFile();
                break;
            case 1:
                if (splitted[0] != "default_error_page")
                    throw BadFile();
                break;
            case 2:
                if (splitted[0] != "client_body_size_max" || !this->_strIsDigit(splitted[1]))
                    throw BadFile();
                break;
            case 3:
                if (splitted[0] != "max_clients")
                    throw BadFile();
                break;
            case 4:
                if (splitted[0] != "routes" || !this->_checkRoutes(file))
                    throw BadFile();
                break;
            default:
                throw BadFile();
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
        throw BadFile();
    }
}