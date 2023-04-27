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
        std::vector<unsigned int> _ports;
        bool _ports_is_set;

        void _setPorts(std::string &ports) {
            if (this->_ports_is_set) {
                throw ErrorParsing("Error: Ports already set");
            }
            std::vector<std::string> ports_str = ParsingUtils::split(ports, ",");
            this->_ports_is_set = true;
            for (unsigned int i = 0; i < ports_str.size(); i++) {
                this->_ports.push_back(ParsingUtils::strToPositiveInteger(ports_str[i]));
            }
        }

        void _setMaxClients(const std::string &maxClients) {
            int num;
            if (this->_maxClients_is_set) {
                throw ErrorParsing("Error: Max clients already set");
            }
            this->_maxClients_is_set = true;
            num = ParsingUtils::strToPositiveInteger(maxClients);
            this->_maxClients = num;
        }

        void _setDefaultErrorPage(const std::string &defaultErrorPage) {
            if (access(defaultErrorPage.c_str(), F_OK) == -1) {
                throw ErrorParsing("Error: Cannot access file default error page");
            }
            if (this->_defaultErrorPage_is_set) {
                throw ErrorParsing("Error: Default error page already set");
            }
            this->_defaultErrorPage_is_set = true;
            this->_defaultErrorPage = defaultErrorPage;
        }

        void _setClientBodyMaxSize(const std::string &clientBodyMaxSize) {
            int num;
            if (this->_clientBodyMaxSize_is_set) {
                throw ErrorParsing("Error: Client body max size already set");
            }
            this->_clientBodyMaxSize_is_set = true;
            num = ParsingUtils::strToPositiveInteger(clientBodyMaxSize);
            this->_clientBodyMaxSize = num;
        }

    public:
        _TempConfiguration() {
            this->_maxClients_is_set = false;
            this->_defaultErrorPage_is_set = false;
            this->_clientBodyMaxSize_is_set = false;
            this->_ports_is_set = false;
        }

        ~_TempConfiguration() {
        }

        std::vector<unsigned int> getPorts() const {
            return this->_ports;
        }

        unsigned int getMaxClients() const {
            return this->_maxClients;
        }

        std::string getDefaultErrorPage() const {
            return this->_defaultErrorPage;
        }

        unsigned int getClientBodyMaxSize() const {
            return this->_clientBodyMaxSize;
        }

        bool checkAllFieldsSet() const {
            if (!this->_maxClients_is_set) {
                throw BadFile("Error: Max clients not set");
            }
            if (!this->_defaultErrorPage_is_set) {
                throw BadFile("Error: Default error page not set");
            }
            if (!this->_clientBodyMaxSize_is_set) {
                throw BadFile("Error: Client body max size not set");
            }
            if (!this->_ports_is_set) {
                throw BadFile("Error: Port not set");
            }
            return true;
        }

        void setFields(const std::string &field, std::string value) {
            if (field == "max_clients") {
                this->_setMaxClients(value);
            } else if (field == "default_error_page") {
                _setDefaultErrorPage(value);
            } else if (field == "client_body_max_size") {
                this->_setClientBodyMaxSize(value);
            } else if (field == "ports") {
                this->_setPorts(value);
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

    class _TempRoute {
    private:
        std::string _index;
        bool _index_is_set;

        std::vector<std::string> _methods;
        bool _methods_is_set;

        std::string _path;
        bool _path_is_set;

        void _setIndex(const std::string &index) {
            if (access(index.c_str(), F_OK) == -1) {
                throw ErrorParsing("Error: Cannot access file index");
            }
            if (this->_index_is_set) {
                throw ErrorParsing("Error: Index already set");
            }
            this->_index_is_set = true;
            this->_index = index;
        }


        void _setMethods(const std::string &methods_input) {
            int number_get = 0;
            int number_post = 0;
            int number_delete = 0;
            std::vector<std::string> methods = ParsingUtils::split(methods_input, ",");
            if (this->_methods_is_set) {
                throw ErrorParsing("Error: Methods already set");
            }
            if (methods.empty()) {
                throw ErrorParsing("Error: No method provided");
            }
            if (methods.size() > 3) {
                throw ErrorParsing("Error: Too many methods");
            }
            for (unsigned int i = 0; i < methods.size(); i++) {
                if (methods[i] != "GET" && methods[i] != "get"
                    && methods[i] != "POST" && methods[i] != "post"
                    && methods[i] != "DELETE" && methods[i] != "delete") {
                    throw ErrorParsing("Error: Invalid method");
                }
                if (methods[i] == "GET" || methods[i] == "get") {
                    number_get++;
                } else if (methods[i] == "POST" || methods[i] == "post") {
                    number_post++;
                } else if (methods[i] == "DELETE" || methods[i] == "delete") {
                    number_delete++;
                }
            }
            if (number_get > 1 || number_post > 1 || number_delete > 1) {
                throw ErrorParsing("Error: Doublons in methods");
            }
            this->_methods_is_set = true;
            this->_methods = methods;
        }

        void _setPath(const std::string &path) {
            if (this->_path_is_set) {
                throw ErrorParsing("Error: route: path already set");
            }
            this->_path_is_set = true;
            this->_path = path;
        }

    public:
        _TempRoute() {
            this->_path_is_set = false;
            this->_methods_is_set = false;
            this->_index_is_set = false;
        }

        ~_TempRoute() {
        }

        std::string getIndex() const {
            return this->_index;
        }

        std::string getPath() const {
            return this->_path;
        }

        std::vector<std::string> getMethods() const {
            return this->_methods;
        }

        bool checkAllFieldsSet() {
            if (!this->_path_is_set) {
                throw BadFile("Error: Route: path is not set");
            }
            if (!this->_methods_is_set) {
                throw BadFile("Error: Route: methods is not set");
            }
            if (!this->_index_is_set) {
                throw BadFile("Error: Route: index is not set");
            }
            return true;
        }

        void setFields(const std::string &field, const std::string &value) {
            if (field == "methods") {
                _setMethods(value);
            } else if (field == "path") {
                this->_setPath(value);
            } else if (field == "index") {
                this->_setIndex(value);
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

    static std::pair<std::string, std::string> _lineToPair(std::string line);

    static Configuration _toConfiguration(_TempConfiguration &server, const std::vector<Route> &routes);

    static std::vector<Configuration> _modelToConfiguration(std::vector<std::pair<_TempConfiguration,
            std::vector<Route> > > model);

    std::vector<std::pair<std::string, std::string> >
    _fieldExtractor(const std::string &line, const std::string &opener);

    static std::vector<Route> _tmpToRoute(std::vector<_TempRoute> data);

    std::vector<Route> _dataToRoute(std::vector<std::string> data);

    _TempConfiguration
    _dataToConfiguration(const std::string &data);

    std::vector<std::pair<_TempConfiguration, std::vector<Route> > >
    _dataToModel(std::vector<std::pair<std::string, std::vector<std::string> > > data);

    static std::vector<std::pair<std::string, std::vector<std::string> > >
    _extractRoute(std::vector<std::string> servers);

    static unsigned int _findCloseBrace(std::string str);

    static std::vector<std::string> _serverSplitter(const std::string &file);

public:
    ConfigurationParser();

    ~ConfigurationParser();

    std::vector<Configuration> parse(const std::string &path);

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