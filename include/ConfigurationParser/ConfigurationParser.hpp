#ifndef CONFIGURATION_PARSER
#define CONFIGURATION_PARSER

#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include "Configuration.hpp"
#include "ParsingUtils.hpp"
#include "Route.hpp"

class ConfigurationParser {
private:
    class _TempConfiguration {
    private:
        unsigned int _maxClients;
        bool _maxClients_is_set;
        std::string _defaultErrorPage;
        bool _defaultErrorPage_is_set;
        unsigned int _clientBodyMaxSize;
        bool _clientBodyMaxSize_is_set;

        void _setMaxClients(std::string maxClients) {
            int num;
            if (this->_maxClients_is_set == true) {
                throw ErrorParsing("Error: Max clients already set");
            }
            this->_maxClients_is_set = true;
            num = ParsingUtils::strToPositiveInteger(maxClients);
            this->_maxClients = num;
        }

        void _setDefaultErrorPage(std::string defaultErrorPage) {
            if (access(defaultErrorPage.c_str(), F_OK) == -1) {
                throw ErrorParsing("Error: Canno't access file default error page");
            }
            if (this->_defaultErrorPage_is_set == true) {
                throw ErrorParsing("Error: Default error page already set");
            }
            this->_defaultErrorPage_is_set = true;
            this->_defaultErrorPage = defaultErrorPage;
        }

        void _setClientBodyMaxSize(std::string clientBodyMaxSize) {
            int num;
            if (this->_clientBodyMaxSize_is_set == true) {
                throw ErrorParsing("Error: Client body max size already set");
            }
            this->_clientBodyMaxSize_is_set = true;
            num = ParsingUtils::strToPositiveInteger(clientBodyMaxSize);
            this->_clientBodyMaxSize = num;
        }

        //std::vector<route> _routes;
    public:
        _TempConfiguration() {
            this->_maxClients_is_set = false;
            this->_defaultErrorPage_is_set = false;
            this->_clientBodyMaxSize_is_set = false;
        }

        ~_TempConfiguration() {
        }

        unsigned int getMaxClients() const {
            return this->_maxClients;
        }

        bool isMaxClientsSet() const {
            return this->_maxClients_is_set;
        }

        std::string getDefaultErrorPage() const {
            return this->_defaultErrorPage;
        }

        bool isDefaultErrorPageSet() const {
            return this->_defaultErrorPage_is_set;
        }

        unsigned int getClientBodyMaxSize() const {
            return this->_clientBodyMaxSize;
        }

        bool isClientBodyMaxSizeSet() const {
            return this->_clientBodyMaxSize_is_set;
        }

        void setFields(std::string field, std::string value) {
            if (field == "max_clients") {
                this->_setMaxClients(value);
            } else if (field == "default_error_page") {
                _setDefaultErrorPage(value);
            } else if (field == "client_body_max_size") {
                this->_setClientBodyMaxSize(value);
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
            }
        };
    };

    _TempConfiguration _splitArgs(std::vector<std::string> &server);

    std::vector<std::vector<std::string> > _splitServers(std::vector<std::string> &file);

    std::vector<std::string>::iterator _findCloseDelimiter(std::vector<std::string> &file,
                                                           char open_limiter, char close_limiter,
                                                           std::vector<std::string>::iterator it);

    std::pair<std::string, std::string> _lineToPair(std::string line);

    std::vector<ConfigurationParser::_TempConfiguration> _splitAllArgs(std::vector<std::vector<std::string> > &servers);

    Configuration _toConfiguration(_TempConfiguration &server) {
        Configuration conf(server.getMaxClients(), server.getDefaultErrorPage(),
                           server.getClientBodyMaxSize());
        return conf;
    }

    std::vector<Configuration> _toVectorConfiguration(std::vector<_TempConfiguration> &servers) {
        std::vector<Configuration> confs;
        for (std::vector<_TempConfiguration>::iterator it = servers.begin(); it != servers.end(); it++) {
            confs.push_back(this->_toConfiguration(*it));
        }
        return confs;
    }

public:
    ConfigurationParser();

    ~ConfigurationParser();

    std::vector<Configuration> parse(std::string config_file) {
        std::string extract_file = ParsingUtils::fileToString(config_file);

        ParsingUtils::removeAllSpace(extract_file);
        std::vector<std::string> file = ParsingUtils::split(extract_file, "\n");
        std::vector<std::string> file2 = ParsingUtils::removeEmptyLine(file);
        ParsingUtils::checkLimiter(file2, '{', '}');
        ParsingUtils::checkLimiter(file2, '[', ']');
        std::vector<std::vector<std::string> > servers = _splitServers(file2);

        std::vector<_TempConfiguration> tmp_vec = _splitAllArgs(servers);
        return _toVectorConfiguration(tmp_vec);
    }

    bool checkAllFieldsSet(_TempConfiguration tmp);

    class BadFile : public std::exception {
    private:
        const char *_msg;
    public:
        BadFile(const char *msg) : _msg(msg) {};

        virtual const char *what() const throw() {
            return (this->_msg);
        };
    };

    std::vector<std::string> _extractArrayField(std::vector<std::string> &vector, std::string to_find);

    void _splitExtractRoutes(std::vector<std::string> &server);

    void _splitExtractPort(std::vector<std::string> &server);


    std::vector<std::string, std::allocator<std::string> >::iterator
    _findField(std::string field_name, std::vector<std::string> &server);
};

#endif