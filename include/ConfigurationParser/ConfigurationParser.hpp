#ifndef CONFIGURATION_PARSER
#define CONFIGURATION_PARSER

#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>

class ConfigurationParser {
private:
    class _TempConfiguration {
    private:
        unsigned int *_port;
        unsigned int *_maxClients;
        std::string *_defaultErrorPage;
        unsigned int *_clientBodyMaxSize;

        void _setPort(int port) {
            if (this->_port != nullptr) {
                throw ErrorParsing("Error: Port already set");
            }
            this->_port = new unsigned int(port);
        }

        void _setMaxClients(unsigned int maxClients) {
            if (this->_maxClients != nullptr) {
                throw ErrorParsing("Error: Max clients already set");
            }
            this->_maxClients = new unsigned int(maxClients);
        }

        void _setDefaultErrorPage(std::string defaultErrorPage) {
            if (access(defaultErrorPage.c_str(), F_OK) == -1) {
                throw ErrorParsing("Error: Canno't access file default error page");
            }
            if (this->_defaultErrorPage != nullptr) {
                throw ErrorParsing("Error: Default error page already set");
            }
            this->_defaultErrorPage = new std::string(defaultErrorPage);
        }

        void _setClientBodyMaxSize(unsigned int clientBodyMaxSize) {
            if (this->_clientBodyMaxSize != nullptr) {
                throw ErrorParsing("Error: Client body max size already set");
            }
            this->_clientBodyMaxSize = new unsigned int(clientBodyMaxSize);
        }

        void _setUnsignedIntField(unsigned int *field, std::string value) {
            int num;
            if (field != nullptr) {
                throw ErrorParsing("Error: Field already set");
            }
            try {
                num = std::stoi(value);
                if (num < 0) {
                    throw std::exception();
                }
                field = new unsigned int(num);
            } catch (std::exception &e) {
                throw ErrorParsing("Error: Invalid value for field: ");
            }
        }
        //std::vector<route> _routes;
    public:
        _TempConfiguration() {
            this->_port = nullptr;
            this->_maxClients = nullptr;
            this->_defaultErrorPage = nullptr;
            this->_clientBodyMaxSize = nullptr;
        }

        ~_TempConfiguration() {
            if (this->_port != nullptr) { delete this->_port; }
            if (this->_maxClients != nullptr) { delete this->_maxClients; }
            if (this->_defaultErrorPage != nullptr) { delete this->_defaultErrorPage; }
            if (this->_clientBodyMaxSize != nullptr) { delete this->_clientBodyMaxSize; }
        };

        unsigned int *getPort() const {
            return this->_port;
        };

        unsigned int *getMaxClients() const {
            return this->_maxClients;
        }

        std::string *getDefaultErrorPage() const {
            return this->_defaultErrorPage;
        }

        unsigned int *getClientBodyMaxSize() const {
            return this->_clientBodyMaxSize;
        }

        void setFields(std::string field, std::string value) {
            if (field == "port") {
                _setUnsignedIntField(this->_port, value);
            } else if (field == "max_clients") {
                _setUnsignedIntField(this->_maxClients, value);
            } else if (field == "default_error_page") {
                _setDefaultErrorPage(value);
            } else if (field == "client_body_max_size") {
                _setUnsignedIntField(this->_clientBodyMaxSize, value);
            } else {
                throw ErrorParsing("Error: Invalid field");
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

    _TempConfiguration _splitArgs(std::vector<std::string> &server);

public:
    ConfigurationParser();

    ~ConfigurationParser();

    std::vector<std::vector<std::string> > splitServers(std::vector<std::string> &file);

    std::vector<std::string>::iterator findServerCloseBraceLine(std::vector<std::string> &file,
                                                                std::vector<std::string>::iterator it);

    std::pair<std::string, std::string> lineToPair(std::string line);

    bool _splitAllArgs(std::vector<std::vector<std::string> > &server);

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