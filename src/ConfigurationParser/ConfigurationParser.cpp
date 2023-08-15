/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationParser.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efournou <efournou@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 20:16:09 by efournou          #+#    #+#             */
/*   Updated: 2023/05/16 20:16:11 by efournou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigurationParser/ConfigurationParser.hpp"
#include "ConfigurationParser/utils/ParsingUtils.hpp"
#include "ConfigurationParser/utils/_TempConfiguration.hpp"
#include "ConfigurationParser/utils/_TempRoute.hpp"
#include "Route.hpp"

void ConfigurationParser::_checkDoubleHost(const VECTOR_CONFIG & configs) {
    for (UINT i = 0; i < configs.size(); i++) {
        for (UINT j = i + 1; j < configs.size(); j++) {
            if (configs[i].host == configs[j].host) {
                throw BadFile("Error: Double host");
            }
        }
    }
}

void ConfigurationParser::_accessRoutePaths(const std::string & location_path,
                                            const std::string & route_index_path){
    if (access(location_path.c_str(), F_OK) == -1) {
        throw BadFile("Error: Cannot access location path");
    }
    if (access(route_index_path.c_str(), F_OK) == -1) {
        throw BadFile("Error: Cannot access route index path");
    }
}

void ConfigurationParser::_accessGeneralPaths(const std::string & root_path,
                                            const std::string & index_path,
                                            const std::string & error_page_path) {
    if (access(root_path.c_str(), F_OK) == -1) {
        throw BadFile("Error: Cannot access root path");
    }
    if (access(index_path.c_str(), F_OK) == -1) {
        throw BadFile("Error: Cannot access index path");
    }
    if (access(error_page_path.c_str(), F_OK) == -1) {
        throw BadFile("Error: Cannot access error page path");
    }
}

void ConfigurationParser::_accessPaths(FINAL_MODEL & model){
    for (UINT i = 0; i < model.size(); i++) {
        std::string root_path       = model[i].first.getRoot();
        std::string index_path      = model[i].first.getIndex();
        if (index_path[0] == '/') {
            index_path.erase(index_path.begin());
        }
        std::string error_page_path = model[i].first.getDefaultErrorPage();
        if (error_page_path[0] == '/') {
            error_page_path.erase(error_page_path.begin());
        }
        index_path      = root_path + "/" + index_path;
        error_page_path = root_path + "/" + error_page_path;
        _accessGeneralPaths(root_path, index_path, error_page_path);
        model[i].first.forceSetRoot(root_path);
        model[i].first.forceSetIndex(index_path);
        model[i].first.forceSetDefaultErrorPage(error_page_path);
        for (UINT j = 0; j < model[i].second.size(); j++) {
            std::string location_path = model[i].second[j].location;
            std::string route_index_path = model[i].second[j].index;
            _accessRoutePaths(location_path, route_index_path);
        }
    }
}

Configuration ConfigurationParser::_toConfiguration
		(TEMP_CONFIGURATION &server, const VECTOR_ROUTE &routes) {
    Configuration conf(server.getHost(),
		server.getMaxClients(),
		server.getDefaultErrorPage(),
		server.getPorts(),
		server.getClientBodyMaxSize(),
        server.getRoot(),
        server.getIndex(),
        server.getCgiPath(),
        server.getCgiExtension(),
		routes);
    return conf;
}

PAIR_STRING ConfigurationParser::_lineToPair(STRING line) {
    VECTOR_STRING	tmp;
    PAIR_STRING		result;

    if (line.size() <= 1) {
        throw (BadFile("Error: Bad server format: Bad arguments"));
    }
    line.erase(line.end());
    tmp = ParsingUtils::split(line, ":");
    if (tmp.size() != 2) {
        throw (BadFile("Error: Bad server format: Bad number of arguments"));
    }
    result.first = tmp[0];
    result.second = tmp[1];
    return result;
}

VECTOR_CONFIG
ConfigurationParser::_modelToConfiguration(FINAL_MODEL model) {
    VECTOR_CONFIG    config;

    for (UINT i = 0; i < model.size(); i++) {
        config.push_back(_toConfiguration(model[i].first, model[i].second));
    }
    return config;
}

ConfigurationParser::FIELDS_MODEL
ConfigurationParser::_fieldExtractor(const STRING &line,
		const STRING &opener) {
    STRING          removed_limiter_line;
    VECTOR_STRING   splitted_line;
    FIELDS_MODEL    fields;

    if (line.find(opener) != 0) {
        throw BadFile("Error: Bad limiters");
    }
    removed_limiter_line = line.substr(opener.length(), line.length() - opener.length() - 1);
    splitted_line = ParsingUtils::split(removed_limiter_line, ";");
    for (UINT i = 0; i < splitted_line.size(); i++) {
        fields.push_back(this->_lineToPair(splitted_line[i]));
    }
    return fields;
}

bool ConfigurationParser::_checkDoubleRoute(VECTOR_ROUTE &routes) {
    for (UINT i = 0; i < routes.size(); i++) {
        for (UINT j = i + 1; j < routes.size(); j++) {
            if (routes[i].location == routes[j].location) {
                return true;
            }
        }
    }
    return false;
}

VECTOR_ROUTE ConfigurationParser::_tmpToRoute(VECTOR_TEMP_ROUTE data, std::string root) {
    VECTOR_ROUTE	routes;
    std::string     root_path = root;

    for (UINT i = 0; i < data.size(); i++) {
        std::string location_path = data[i].getLocation();
            if (location_path[0] == '/') {
                location_path.erase(location_path.begin());
            }
            if (location_path.back() == '/') {
                location_path.erase(location_path.end() - 1);
            }
            location_path = root_path + "/" + location_path;
            if (location_path.back() == '/') {
                location_path.erase(location_path.end() - 1);
            }
            std::string route_index_path = data[i].getIndex();
            if (route_index_path[0] == '/') {
                route_index_path.erase(route_index_path.begin());
            }
            route_index_path = location_path + "/" + route_index_path;
            if (location_path == root_path){
                location_path += "/";
            }
        routes.push_back(Route(route_index_path, data[i].getMethods(), location_path, data[i].getRedirection()));
    }
    if (_checkDoubleRoute(routes)) {
        throw BadFile("Error: Bad route format: Both route have the same location");
    }
    return routes;
}

VECTOR_ROUTE ConfigurationParser::_dataToRoute(VECTOR_STRING data, std::string root) {
    VECTOR_TEMP_ROUTE	tmp_routes;
    FIELDS_MODEL	    fields;

    for (UINT i = 0; i < data.size(); i++) {
        fields = _fieldExtractor(data[i], "route{");
        tmp_routes.push_back(ConfigurationParser::_TempRoute());
        for (UINT j = 0; j < fields.size(); j++) {
            tmp_routes[i].setFields(fields[j].first, fields[j].second);
        }
        tmp_routes[i].checkAllFieldsSet();
    }
    VECTOR_ROUTE routes = VECTOR_ROUTE(_tmpToRoute(tmp_routes, root));
    return routes;
}

ConfigurationParser::TEMP_CONFIGURATION
ConfigurationParser::_dataToConfiguration(const STRING &data) {
    TEMP_CONFIGURATION	config;
    FIELDS_MODEL	    fields;

    fields = _fieldExtractor(data, "server{");
    for (UINT i = 0; i < fields.size(); i++) {
        config.setFields(fields[i].first, fields[i].second);
    }
    config.checkAllFieldsSet();
    return config;
}

ConfigurationParser::FINAL_MODEL ConfigurationParser::_dataToModel(EXTRACTED_ROUTE_MODEL data) {
    FINAL_MODEL model;
    std::string root;

    for (UINT i = 0; i < data.size(); i++) {
        TEMP_CONFIGURATION config = _dataToConfiguration(data[i].first);
        root = config.getRoot();
        VECTOR_ROUTE routes = _dataToRoute(data[i].second, root);
        model.push_back(std::make_pair(config, routes));
    }
    return model;
}

UINT ConfigurationParser::_findCloseBrace(STRING str) {
    UINT	open_brace	= 0;
    UINT	close_brace	= 0;

    for (UINT i = 0; i < str.size(); i++) {
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

ConfigurationParser::EXTRACTED_ROUTE_MODEL
ConfigurationParser::_extractRoute(VECTOR_STRING servers) {
    UINT		            close_route_brace;
    UINT		            open_route_brace;
    VECTOR_STRING		    routes;
    EXTRACTED_ROUTE_MODEL	return_pair;

    for (UINT i = 0; i < servers.size(); i++) {
        routes.clear();
        while (servers[i].find("route{") != servers[i].npos) {
            open_route_brace = servers[i].find("route{");
            close_route_brace = _findCloseBrace(servers[i].substr(open_route_brace));
            routes.push_back(servers[i].substr(open_route_brace, close_route_brace + 1));
            servers[i].erase(open_route_brace, close_route_brace + 1);
        }
        return_pair.push_back(std::make_pair(servers[i], routes));
    }
    return return_pair;
}

VECTOR_STRING ConfigurationParser::_serverSplitter(const STRING &file) {
    VECTOR_STRING   servers;

    if (file.substr(0, 7) != "server{") {
        throw BadFile("Error: first line is not server{");
    }
    for (UINT i = 0; i < file.size(); i++) {
        servers.push_back(file.substr(i, _findCloseBrace(file.substr(i)) + 1));
        i += _findCloseBrace(file.substr(i));
    }
    return servers;
}

VECTOR_CONFIG ConfigurationParser::parse(const STRING &path) {
    STRING                  file                = ParsingUtils::fileToString(path);
    ParsingUtils::checkLimiter(file);
    STRING                  removed_space       = ParsingUtils::removeIsSpace(file);
    SPLITTED_FILE           servers             = _serverSplitter(removed_space);
    EXTRACTED_ROUTE_MODEL   pair_server_route   = _extractRoute(servers);
    FINAL_MODEL             temp_model          = _dataToModel(pair_server_route);
    _accessPaths(temp_model);
    VECTOR_CONFIG           configs             = _modelToConfiguration(temp_model);
    _checkDoubleHost(configs);
    return configs;
}

ConfigurationParser::ConfigurationParser() {}

ConfigurationParser::~ConfigurationParser() {}
