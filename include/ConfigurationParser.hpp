#ifndef CONFIGURATION_PARSER
#define CONFIGURATION_PARSER

#include <iostream>
#include <fstream>
#include <vector>
class ConfigurationParser{
    private:
        bool _checkFile(std::ifstream & file);
        bool _checkServer(std::ifstream & file);
        bool _strContainsSpace(std::string str);
        bool _vectorContainsSpace(std::vector <std::string> vec);
        bool _strIsDigit(std::string str);
        bool _checkRoutes(std::ifstream & file);
    public:
        ConfigurationParser();
        ~ConfigurationParser();

        std::vector <std::string> _split(std::string str, std::string delimiter);
        void fromFile(std::string path);
        class BadFile : public std::exception
        {
            private:
            const char * _msg;
            public:
            BadFile(const char * msg) : _msg(msg) {};
            virtual const char* what() const throw()
            {
                return (this->_msg);
            };
        };
};

#endif