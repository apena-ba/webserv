#include "ConfigurationParser/ConfigurationParser.hpp"
#include "ConfigurationParser/ParsingUtils.hpp"
#include "Route.hpp"

bool ConfigurationParser::checkAllFieldsSet(_TempConfiguration tmp) {
    if (tmp.isMaxClientsSet() == false) {
        throw BadFile("Error: Max clients not set");
    }
    if (tmp.isDefaultErrorPageSet() == false) {
        throw BadFile("Error: Default error page not set");
    }
    if (tmp.isClientBodyMaxSizeSet() == false) {
        throw BadFile("Error: Client body max size not set");
    }
    return true;
}

std::vector<std::string>::iterator
ConfigurationParser::_findField(std::string field_name, std::vector<std::string> &server) {
    std::vector<std::string>::iterator field_it;
    unsigned int number_field = 0;
    for (std::vector<std::string>::iterator it = server.begin() + 1; it != server.end() - 1; ++it) {
        if ((*it).compare(0, field_name.size(), field_name) == 0) {
            number_field++;
            field_it = it;
        }
    }
    if (number_field != 1) { throw BadFile("Error: Server should contain 1 field port"); }
    //std::cout << "field: " << *field_it << std::endl;
    return field_it;
}

/*
bool ConfigurationParser::checkRoutes(std::string routes) {
    if (ParsingUtils::strContainsChar(routes, ':') == false) {
        throw BadFile("Error: Routes not set");
    }
    return true;
}

bool ConfigurationParser::checkPort(std::string port) {
    if (ParsingUtils::strContainsChar(port, ':') == false) {
        throw BadFile("Error: Port not set");
    }
    return true;
}*/

std::vector<std::string>
ConfigurationParser::_extractArrayField(std::vector<std::string> &server, std::string to_find) {
    std::vector<std::string>::iterator field_begin_it;
    std::vector<std::string>::iterator field_end_it;
    std::vector<std::string> array_field;

    field_begin_it = _findField(to_find, server);
    field_end_it = _findCloseDelimiter(server, '[', ']', field_begin_it);
    for (unsigned int dis = 0; dis <= std::distance(field_begin_it, field_end_it); dis++) {
        array_field.push_back(*field_begin_it);
        server.erase(field_begin_it);
    }
    return array_field;
}

void ConfigurationParser::_splitExtractRoutes(std::vector<std::string> &server) {
    std::vector<std::string>::iterator field_it;
    std::vector<std::string> routes;

    routes = _extractArrayField(server, "routes:");
    if (routes[0][7] != '[') {
        throw BadFile("Error: Bad server format: routes: must be followed by [");
    }
    if (routes[0].size() != 8) {
        throw BadFile("Error: Bad server format: routes: put new line for every field");
    }
}

void ConfigurationParser::_splitExtractPort(std::vector<std::string> &server) {
    std::vector<std::string>::iterator field_it;
    std::vector<std::string> ports;

    ports = _extractArrayField(server, "ports:");
    if (ports[0][6] != '[') {
        throw BadFile("Error: Bad server format: ports: must be followed by [");
    }
}

std::vector<ConfigurationParser::_TempConfiguration>
ConfigurationParser::_splitAllArgs(std::vector<std::vector<std::string> > &servers) {
    std::vector<_TempConfiguration> result;
    _TempConfiguration tmp;
    unsigned int *port;
    Route *routes;

    for (unsigned int i = 0; i < servers.size(); i++) {
        _splitExtractPort(servers[i]);
        _splitExtractRoutes(servers[i]);
        /*tmp = _splitArgs(servers[i]);
        checkAllFieldsSet(tmp);
        result.push_back(tmp);*/
    }
    return result;
}

std::pair<std::string, std::string> ConfigurationParser::_lineToPair(std::string line) {
    std::vector<std::string> tmp;
    std::pair<std::string, std::string> result;

    if (line.size() <= 1) {
        throw (BadFile("Error: Bad server format: Bad arguments"));
    }
    line.erase(line.end() - 1);
    tmp = ParsingUtils::split(line, ":");
    if (tmp.size() != 2) {
        throw (BadFile("Error: Bad server format: Bad number of arguments"));
    }
    result.first = tmp[0];
    result.second = tmp[1];
    return result;
}

ConfigurationParser::_TempConfiguration ConfigurationParser::_splitArgs(std::vector<std::string> &server) {
    std::pair<std::string, std::string> arg;
    _TempConfiguration tmp_config;

    if (server[0].compare(0, 7, "server{") != 0 || server[0].size() != 7) {
        throw BadFile("Error: Bad server format: First line must be server{");
    }
    for (std::vector<std::string>::iterator it = server.begin() + 1; it != server.end() - 1; ++it) {
        if ((*it).back() != ';') {
            throw (BadFile("Error: Bad server format: Put a semicolon at the end of each line"));
        }
        arg = _lineToPair(*it);
        tmp_config.setFields(arg.first, arg.second);
    }
    if (server.back() == "}" && server.back().size() == 1) {
        return tmp_config;
    }
    if (server.back().back() != '}') {
        throw (BadFile("Error: Bad server format: Last line must be }"));
    }
    if (server.back()[server.back().size() - 2] != ';') {
        throw (BadFile("Error: Bad server format: Put a semicolon at the end of each line"));
    }
    arg = _lineToPair(server.back());
    tmp_config.setFields(arg.first, arg.second);
    return tmp_config;
}


std::vector<std::string>::iterator ConfigurationParser::_findCloseDelimiter(std::vector<std::string> &file,
                                                                            char open_limiter, char close_limiter,
                                                                            std::vector<std::string>::iterator it) {
    unsigned int open_limiter_number = 0;
    unsigned int close_limiter_number = 0;

    for (; it != file.end(); ++it) {
        for (unsigned long j = 0; j < (*it).size(); ++j) {
            if ((*it)[j] == open_limiter) {
                open_limiter_number++;
            } else if ((*it)[j] == close_limiter) {
                close_limiter_number++;
            }
            if (open_limiter_number == close_limiter_number && open_limiter_number != 0 && close_limiter_number != 0) {
                if ((*it).size() - 1 != j) {
                    throw ParsingUtils::ErrorParsing("Error: Bad server format: "
                                                     "Put a new line after closing delimiter");
                }
                return it;
            }
        }
    }
    throw ParsingUtils::ErrorParsing("Error: Bad server format: No closing brace");
}

std::vector<std::vector<std::string> > ConfigurationParser::_splitServers(std::vector<std::string> &file) {
    std::vector<std::string>::iterator iterator_close_brace;
    std::vector<std::vector<std::string> > servers;
    for (std::vector<std::string>::iterator it = file.begin(); it != file.end(); ++it) {
        iterator_close_brace = _findCloseDelimiter(file, '{', '}', it);
        std::vector<std::string> server;
        server = std::vector<std::string>(it, iterator_close_brace + 1);
        servers.push_back(server);
        it = iterator_close_brace;
    }
    return servers;
}

ConfigurationParser::ConfigurationParser() {}

ConfigurationParser::~ConfigurationParser() {}

