#include "ConfigurationParser/ConfigurationParser.hpp"
#include "ConfigurationParser/ParsingUtils.hpp"

bool ConfigurationParser::checkServers(std::vector<std::vector<std::string> > &server) {
    for (unsigned int i = 0; i < server.size(); i++) {
        if (_checkServer(server[i]) == false) {
            std::cout << "Error: Bad server format" << std::endl;
            break;
        }
        std::cout << "Server format OK" << std::endl;
    }
    return true;
}

bool ConfigurationParser::_checkServer(std::vector<std::string> &server) {
    if (server[0] != "server{" || server.back().back() != '}') {
        if (server[0] != "server{") {
            throw BadFile("Error: Bad server format: First line is not server{");
        } else if (server.back().back() != '}') {
            throw BadFile("Error: Bad server format: Last line is not }");
        }
        return false;
    }
    return true;
}

std::vector<std::vector<std::string> > ConfigurationParser::splitServers(std::vector<std::string> &file) {
    std::vector<std::vector<std::string> > result;
    std::vector<std::string> server;
    for (unsigned int i = 0; i < file.size(); i++) {
        if (file[i] == "server{") {
            for (unsigned int j = 0; j < file.size(); j++) {
                if (ParsingUtils::strContainsChar(file[j + i], '[')) {
                    for (unsigned int k = j; k < file.size(); k++) {
                        if (ParsingUtils::strContainsChar(file[k + i], ']')) {
                            server.push_back(file[k + i]);
                            j = k;
                            break;
                        } else if (file[k + i] == file.back() ||
                                   (ParsingUtils::strContainsChar(file[k + i], '[') && k > 0)) {
                            throw BadFile("Error: Bad server format: Not closed brackets");
                        } else {
                            server.push_back(file[k + i]);
                        }
                    }
                } else if (ParsingUtils::strContainsChar(file[j + i], '}')) {
                    server.push_back(file[j + i]);
                    result.push_back(server);
                    server.clear();
                    i = j + i;
                    break;
                } else if (file[j + i] == file.back() || (ParsingUtils::strContainsChar(file[j + i], '{') && j > 0)) {
                    throw BadFile("Error: Bad server format: Not closed brace");
                } else {
                    server.push_back(file[j + i]);
                }
            }
        }
    }
    return result;
}

ConfigurationParser::ConfigurationParser() {}

ConfigurationParser::~ConfigurationParser() {}
