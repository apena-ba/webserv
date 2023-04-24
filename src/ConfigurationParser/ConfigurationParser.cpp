#include "ConfigurationParser/ConfigurationParser.hpp"
#include "ConfigurationParser/ParsingUtils.hpp"

bool ConfigurationParser::checkAllFieldsSet(_TempConfiguration tmp) {
    if (tmp.isPortSet() == false) {
        throw BadFile("Error: Port not set");
    }
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

std::vector<ConfigurationParser::_TempConfiguration>
ConfigurationParser::_splitAllArgs(std::vector<std::vector<std::string> > &server) {
    std::vector<_TempConfiguration> result;
    _TempConfiguration tmp;

    for (unsigned int i = 0; i < server.size(); i++) {
        tmp = _splitArgs(server[i]);
        checkAllFieldsSet(tmp);
        result.push_back(tmp);
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

std::vector<std::string>::iterator ConfigurationParser::_findServerCloseBraceLine(std::vector<std::string> &file,
                                                                                  std::vector<std::string>::iterator it) {
    unsigned int openBraces = 0;
    unsigned int closeBraces = 0;

    for (; it != file.end(); ++it) {
        for (unsigned long j = 0; j < (*it).size(); ++j) {
            if ((*it)[j] == '{') {
                openBraces++;
            } else if ((*it)[j] == '}') {
                closeBraces++;
            }
            if (openBraces == closeBraces && openBraces != 0 && closeBraces != 0) {
                if ((*it).size() - 1 != j) {
                    throw ParsingUtils::ErrorParsing("Error: Bad server format: "
                                                     "Put a new line after closing brace, line ");
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
        iterator_close_brace = _findServerCloseBraceLine(file, it);
        std::vector<std::string> server;
        server = std::vector<std::string>(it, iterator_close_brace + 1);
        servers.push_back(server);
        it = iterator_close_brace;
    }
    return servers;
}

ConfigurationParser::ConfigurationParser() {}

ConfigurationParser::~ConfigurationParser() {}
