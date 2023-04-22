#ifndef CONFIGURATION_PARSER
#define CONFIGURATION_PARSER

#include <iostream>
#include <fstream>
#include <vector>

class ConfigurationParser {
private:
    bool _checkServer(std::vector<std::string> &server);

public:
    ConfigurationParser();

    ~ConfigurationParser();

    std::vector<std::vector<std::string> > splitServers(std::vector<std::string> &file);

    bool checkServers(std::vector<std::vector<std::string> > &server);

    bool checkBraces(std::vector<std::string> &server);

    void fromFile(std::string path);

    class BadFile : public std::exception {
    private:
        const char *_msg;
    public:
        BadFile(const char *msg) : _msg(msg) {};

        virtual const char *what() const throw() {
            return (this->_msg);
        };
    };
};

#endif