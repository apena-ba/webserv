#ifndef CONFIGURATION_PARSER
#define CONFIGURATION_PARSER

#include <iostream>
#include <fstream>
#include <vector>
class ConfigurationParser{
    private:
        bool _checkFile(std::ifstream & file);
        bool _checkServer(std::ifstream & file);
        bool ConfigurationParser::_strContainsSpace(std::string str);
        bool ConfigurationParser::_vectorContainsSpace(std::vector <std::string> vec);
    public:
        ConfigurationParser();
        ~ConfigurationParser();

        std::vector <std::string> _split(std::string str, std::string delimiter);
        void fromFile(std::string path);
        class BadFile : public std::exception
        {
            virtual const char* what() const throw()
            {
                return ("File doesn't respect the expected format");
            };
        };
};

#endif