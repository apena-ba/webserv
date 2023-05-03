#include "ConfigurationParser/utils/_TempConfiguration.hpp"

void TEMP_CONFIGURATION::_setHost(const STRING &host) {
    if (!this->_host.empty()) {
        throw ErrorParsing("Error: Host already set");
    }
    this->_host = host;
}

void TEMP_CONFIGURATION::_setPorts(STRING &ports) {
    unsigned int num;

    if (!this->_ports.empty()) {
        throw ErrorParsing("Error: Ports already set");
    }
    VECTOR_STRING ports_str = ParsingUtils::split(ports, ",");
    if (ParsingUtils::checkDoubleValue(ports_str)) {
        throw ErrorParsing("Error: Double value in ports");
    }
    for (unsigned int i = 0; i < ports_str.size(); i++) {
        if (!ParsingUtils::betteratoi(ports_str[i].c_str(), num)) {
            throw ErrorParsing("Error: Port is not a unsigned integer");
        }
        this->_ports.push_back(num);
    }
}

void TEMP_CONFIGURATION::_setMaxClients(const STRING &maxClients) {
    unsigned int num;

    if (this->_maxClients_is_set) {
        throw ErrorParsing("Error: Max clients already set");
    }
    this->_maxClients_is_set = true;
    if (!ParsingUtils::betteratoi(maxClients.c_str(), num)) {
        throw ErrorParsing("Error: Port is not a unsigned integer");
    }
    this->_maxClients = num;
}

void TEMP_CONFIGURATION::_setDefaultErrorPage(const STRING &defaultErrorPage) {
    if (access(defaultErrorPage.c_str(), F_OK) == -1) {
        throw ErrorParsing("Error: Cannot access file default error page");
    }
    if (!this->_defaultErrorPage.empty()) {
        throw ErrorParsing("Error: Default error page already set");
    }
    this->_defaultErrorPage = defaultErrorPage;
}

void TEMP_CONFIGURATION::_setClientBodyMaxSize(const STRING &clientBodyMaxSize) {
    unsigned int num;

    if (this->_clientBodyMaxSize_is_set) {
        throw ErrorParsing("Error: Client body max size already set");
    }
    this->_clientBodyMaxSize_is_set = true;
    if (!ParsingUtils::betteratoi(clientBodyMaxSize.c_str(), num)) {
        throw ErrorParsing("Error: Port is not an unsigned integer");
    }
    this->_clientBodyMaxSize = num;
}

TEMP_CONFIGURATION::_TempConfiguration() {
    this->_maxClients_is_set        = false;
    this->_clientBodyMaxSize_is_set = false;
}

TEMP_CONFIGURATION::~_TempConfiguration() {}

STRING TEMP_CONFIGURATION::getHost() const {
    return this->_host;
}

std::vector<unsigned int> TEMP_CONFIGURATION::getPorts() const {
    return this->_ports;
}

unsigned int TEMP_CONFIGURATION::getMaxClients() const {
    return this->_maxClients;
}

STRING TEMP_CONFIGURATION::getDefaultErrorPage() const {
    return this->_defaultErrorPage;
}

unsigned int TEMP_CONFIGURATION::getClientBodyMaxSize() const {
    return this->_clientBodyMaxSize;
}

bool TEMP_CONFIGURATION::checkAllFieldsSet() const {
    if (!this->_maxClients_is_set) {
        throw BadFile("Error: Max clients not set");
    }
    if (this->_defaultErrorPage.empty()) {
        throw BadFile("Error: Default error page not set");
    }
    if (!this->_clientBodyMaxSize_is_set) {
        throw BadFile("Error: Client body max size not set");
    }
    if (this->_ports.empty()) {
        throw BadFile("Error: Port not set");
    }
    return true;
}

void TEMP_CONFIGURATION::setFields(const STRING &field, STRING value) {
    if (field == "host") {
        this->_setHost(value);
    } else if (field == "max_clients") {
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
