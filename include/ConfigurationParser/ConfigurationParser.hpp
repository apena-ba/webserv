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
    class _TmpConfiguration {
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

    public:
        _TmpConfiguration() {
            this->_maxClients_is_set = false;
            this->_defaultErrorPage_is_set = false;
            this->_clientBodyMaxSize_is_set = false;
            this->_ports_is_set = false;
        }

        ~_TmpConfiguration() {
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

        bool checkAllFieldsSet() {
            if (this->_maxClients_is_set == false) {
                throw BadFile("Error: Max clients not set");
            }
            if (this->_defaultErrorPage_is_set == false) {
                throw BadFile("Error: Default error page not set");
            }
            if (this->_clientBodyMaxSize_is_set == false) {
                throw BadFile("Error: Client body max size not set");
            }
            if (this->_ports_is_set == false) {
                throw BadFile("Error: Port not set");
            }
            return true;
        }

        void setFields(std::string field, std::string value) {
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

        void _setIndex(std::string index) {
            if (access(index.c_str(), F_OK) == -1) {
                throw ErrorParsing("Error: Canno't access file index");
            }
            if (this->_index_is_set == true) {
                throw ErrorParsing("Error: Index already set");
            }
            this->_index_is_set = true;
            this->_index = index;
        }


        void _setMethods(std::string methods_input) {
            int number_get = 0;
            int number_post = 0;
            int number_delete = 0;
            std::vector<std::string> methods = ParsingUtils::split(methods_input, ",");
            if (this->_methods_is_set == true) {
                throw ErrorParsing("Error: Methods already set");
            }
            if (methods.size() == 0) {
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
                throw ErrorParsing("Error: Doublon in methods");
            }
            this->_methods_is_set = true;
            this->_methods = methods;
        }

        void _setPath(std::string path) {
            if (this->_path_is_set == true) {
                throw ErrorParsing("Error: path already set");
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
            if (this->_path_is_set == false) {
                throw BadFile("Error: Route: path is not set");
            }
            if (this->_methods_is_set == false) {
                throw BadFile("Error: Route: methods is not set");
            }
            if (this->_index_is_set == false) {
                throw BadFile("Error: Route: index is not set");
            }
            return true;
        }

        void setFields(std::string field, std::string value) {
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

    _TmpConfiguration _splitArgs(std::vector<std::string> &server);

    std::vector<std::vector<std::string> >
    _splitServers(std::vector<std::string> &file, char open_limiter, char close_limiter);

    std::vector<std::string>::iterator _findCloseDelimiter(std::vector<std::string> &file,
                                                           char open_limiter, char close_limiter,
                                                           std::vector<std::string>::iterator it);

    std::pair<std::string, std::string> _lineToPair(std::string line);

    std::vector<ConfigurationParser::_TmpConfiguration> _splitAllArgs(std::vector<std::vector<std::string> > &servers);

    Configuration _toConfiguration(_TmpConfiguration &server, std::vector<Route> routes) {
        Configuration conf(server.getMaxClients(), server.getDefaultErrorPage(), server.getPorts(),
                           server.getClientBodyMaxSize(), routes);
        return conf;
    }

    std::vector<Configuration>
    _toVectorConfiguration(std::vector<_TmpConfiguration> &servers, std::vector<Route> routes) {
        std::vector<Configuration> confs;
        for (std::vector<_TmpConfiguration>::iterator it = servers.begin(); it != servers.end(); it++) {
            confs.push_back(this->_toConfiguration(*it, routes));
        }
        return confs;
    }

public:
    ConfigurationParser();

    ~ConfigurationParser();

    std::vector<Configuration> parse(std::string path) {
        std::string file = ParsingUtils::fileToString(path);
        ParsingUtils::checkLimiter(file);
        ParsingUtils::removeAllSpace(file);
        std::string new_str = ParsingUtils::removeIsSpace(file);
        std::vector<std::string> servers = serverSplitter(new_str);
        std::vector<std::pair<std::string, std::vector<std::string> > >
                pair = extractRoute(servers);

        std::vector<std::pair<_TmpConfiguration,
                std::vector<Route> > > modele = dataToModele(pair);
        std::vector<Configuration> configs = modelToConfiguration(modele);
        return configs;
    }

    std::vector<Configuration> modelToConfiguration(std::vector<std::pair<_TmpConfiguration,
            std::vector<Route> > > modele) {
        std::vector<Configuration> confs;
        for (unsigned int i = 0; i < modele.size(); i++) {
            confs.push_back(_toConfiguration(modele[i].first, modele[i].second));
        }
        return confs;
    }

    std::vector<std::pair<std::string, std::string> > fieldExtractor(std::string line, std::string opener) {
        std::string removed_limiter_line;
        std::vector<std::string> splitted;
        std::vector<std::pair<std::string, std::string> > fields;
        if (line.find(opener) != 0) {
            throw BadFile("Error: Bad limiters");
        }
        removed_limiter_line = line.substr(opener.length(), line.length() - opener.length() - 1);
        splitted = ParsingUtils::split(removed_limiter_line, ";");
        for (unsigned int i = 0; i < splitted.size(); i++) {
            fields.push_back(this->_lineToPair(splitted[i]));
        }
        return fields;
    }

    std::vector<Route> tmpToRoute(std::vector<_TempRoute> data) {
        std::vector<Route> routes;
        for (unsigned int i = 0; i < data.size(); i++) {
            routes.push_back(Route(data[i].getIndex(), data[i].getMethods(), data[i].getPath()));
        }

        return routes;
    }

    std::vector<Route> dataToRoute(std::vector<std::string> data) {
        std::vector<_TempRoute> tmp_routes;
        std::vector<std::pair<std::string, std::string> > fields;

        for (unsigned int i = 0; i < data.size(); i++) {
            fields = fieldExtractor(data[i], "route{");
            tmp_routes.push_back(_TempRoute());
            for (unsigned int j = 0; j < fields.size(); j++) {
                tmp_routes[i].setFields(fields[j].first, fields[j].second);
            }
            tmp_routes[i].checkAllFieldsSet();
        }
        std::vector<Route> routes = std::vector<Route>(tmpToRoute(tmp_routes));
        return routes;
    }

    _TmpConfiguration
    dataToConfiguration(std::string data) {
        _TmpConfiguration confs;
        std::vector<std::pair<std::string, std::string> > fields;
        fields = fieldExtractor(data, "server{");
        for (unsigned int i = 0; i < fields.size(); i++) {
            confs.setFields(fields[i].first, fields[i].second);
        }
        confs.checkAllFieldsSet();
        return confs;
    }

    std::vector<std::pair<_TmpConfiguration, std::vector<Route> > >
    dataToModele(std::vector<std::pair<std::string, std::vector<std::string> > > data) {
        std::vector<std::pair<_TmpConfiguration, std::vector<Route> > > modele;
        for (unsigned int i = 0; i < data.size(); i++) {
            _TmpConfiguration confs = dataToConfiguration(data[i].first);
            std::vector<Route> routes = dataToRoute(data[i].second);
            modele.push_back(std::make_pair(confs, routes));
        }
        return modele;
    }

    std::vector<std::pair<std::string, std::vector<std::string> > > extractRoute(std::vector<std::string> servers) {
        int close_route_brace = 0;
        unsigned int open_route_brace = 0;
        std::vector<std::string> routes;
        std::vector<std::pair<std::string, std::vector<std::string> > > return_pair;

        for (unsigned int i = 0; i < servers.size(); i++) {
            routes.clear();
            while (servers[i].find("route{") != servers[i].npos) {
                open_route_brace = servers[i].find("route{");
                close_route_brace = findCloseBrace(servers[i].substr(open_route_brace));
                routes.push_back(servers[i].substr(open_route_brace, close_route_brace + 1));
                servers[i].erase(open_route_brace, close_route_brace + 1);
            }
            return_pair.push_back(std::make_pair(servers[i], routes));
        }
        return return_pair;
    }

    int findCloseBrace(std::string str) {
        unsigned int open_brace = 0;
        unsigned int close_brace = 0;
        for (unsigned int i = 0; i < str.size(); i++) {
            if (str[i] == '{') {
                open_brace++;
            } else if (str[i] == '}') {
                close_brace++;
            }
            if (open_brace == close_brace && open_brace != 0 && close_brace != 0) {
                return i;
            }
        }
        throw BadFile("Error: not the same number of open and close brace");
    }

    std::vector<std::string> serverSplitter(std::string file) {
        std::vector<std::string> servers;
        if (file.substr(0, 7) != "server{") {
            throw BadFile("Error: first line is not server{");
        }
        for (unsigned int i = 0; i < file.size(); i++) {
            servers.push_back(file.substr(i, findCloseBrace(file.substr(i)) + 1));
            i += findCloseBrace(file.substr(i));
        }
        return servers;
    }

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