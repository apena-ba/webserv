#include "ConfigurationParser/ConfigurationParser.hpp"
#include "ConfigurationParser/ParsingUtils.hpp"
#include "Route.hpp"

Configuration
ConfigurationParser::_toConfiguration(ConfigurationParser::_TempConfiguration &server, std::vector<Route> routes) {
    Configuration conf(server.getMaxClients(), server.getDefaultErrorPage(), server.getPorts(),
                       server.getClientBodyMaxSize(), routes);
    return conf;
}

std::pair<std::string, std::string> ConfigurationParser::_lineToPair(std::string line) {
    std::vector<std::string> tmp;
    std::pair<std::string, std::string> result;

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

std::vector<Configuration>
ConfigurationParser::_modelToConfiguration(std::vector<std::pair<ConfigurationParser::_TempConfiguration,
        std::vector<Route> > > model) {
    std::vector<Configuration> confs;
    for (unsigned int i = 0; i < model.size(); i++) {
        confs.push_back(_toConfiguration(model[i].first, model[i].second));
    }
    return confs;
}

std::vector<std::pair<std::string, std::string> >
ConfigurationParser::_fieldExtractor(std::string line, std::string opener) {
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

std::vector<Route> ConfigurationParser::_tmpToRoute(std::vector<ConfigurationParser::_TempRoute> data) {
    std::vector<Route> routes;
    for (unsigned int i = 0; i < data.size(); i++) {
        routes.push_back(Route(data[i].getIndex(), data[i].getMethods(), data[i].getPath()));
    }

    return routes;
}

std::vector<Route> ConfigurationParser::_dataToRoute(std::vector<std::string> data) {
    std::vector<ConfigurationParser::_TempRoute> tmp_routes;
    std::vector<std::pair<std::string, std::string> > fields;

    for (unsigned int i = 0; i < data.size(); i++) {
        fields = _fieldExtractor(data[i], "route{");
        tmp_routes.push_back(ConfigurationParser::_TempRoute());
        for (unsigned int j = 0; j < fields.size(); j++) {
            tmp_routes[i].setFields(fields[j].first, fields[j].second);
        }
        tmp_routes[i].checkAllFieldsSet();
    }
    std::vector<Route> routes = std::vector<Route>(_tmpToRoute(tmp_routes));
    return routes;
}

ConfigurationParser::ConfigurationParser::_TempConfiguration
ConfigurationParser::_dataToConfiguration(std::string data) {
    ConfigurationParser::_TempConfiguration confs;
    std::vector<std::pair<std::string, std::string> > fields;
    fields = _fieldExtractor(data, "server{");
    for (unsigned int i = 0; i < fields.size(); i++) {
        confs.setFields(fields[i].first, fields[i].second);
    }
    confs.checkAllFieldsSet();
    return confs;
}

std::vector<std::pair<ConfigurationParser::_TempConfiguration, std::vector<Route> > >
ConfigurationParser::_dataToModel(std::vector<std::pair<std::string, std::vector<std::string> > > data) {
    std::vector<std::pair<ConfigurationParser::_TempConfiguration, std::vector<Route> > > model;
    for (unsigned int i = 0; i < data.size(); i++) {
        ConfigurationParser::_TempConfiguration confs = _dataToConfiguration(data[i].first);
        std::vector<Route> routes = _dataToRoute(data[i].second);
        model.push_back(std::make_pair(confs, routes));
    }
    return model;
}

std::vector<std::pair<std::string, std::vector<std::string> > >
ConfigurationParser::_extractRoute(std::vector<std::string> servers) {
    int close_route_brace = 0;
    unsigned int open_route_brace = 0;
    std::vector<std::string> routes;
    std::vector<std::pair<std::string, std::vector<std::string> > > return_pair;

    for (unsigned int i = 0; i < servers.size(); i++) {
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

int ConfigurationParser::_findCloseBrace(std::string str) {
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

std::vector<std::string> ConfigurationParser::_serverSplitter(std::string file) {
    std::vector<std::string> servers;
    if (file.substr(0, 7) != "server{") {
        throw BadFile("Error: first line is not server{");
    }
    for (unsigned int i = 0; i < file.size(); i++) {
        servers.push_back(file.substr(i, _findCloseBrace(file.substr(i)) + 1));
        i += _findCloseBrace(file.substr(i));
    }
    return servers;
}

std::vector<Configuration> ConfigurationParser::parse(std::string path) {
    std::string file = ParsingUtils::fileToString(path);
    ParsingUtils::checkLimiter(file);
    ParsingUtils::removeAllSpace(file);
    std::string new_str = ParsingUtils::removeIsSpace(file);
    std::vector<std::string> servers = _serverSplitter(new_str);
    std::vector<std::pair<std::string, std::vector<std::string> > >
            pair = _extractRoute(servers);

    std::vector<std::pair<ConfigurationParser::_TempConfiguration,
            std::vector<Route> > > model = _dataToModel(pair);
    std::vector<Configuration> configs = _modelToConfiguration(model);
    return configs;
}

ConfigurationParser::ConfigurationParser() {}

ConfigurationParser::~ConfigurationParser() {}

