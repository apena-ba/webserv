#include "ConfigurationParser/utils/_TempConfiguration.hpp"

void ConfigurationParser::TEMP_CONFIGURATION::_setPhpCgiPath(const STRING &phpPath) {
    if (!this->_phpCgiPath.empty()) {
        throw ErrorParsing("Error: PhpCgiPath already set");
    }
    if (access(phpPath.c_str(), F_OK) == -1) {
        throw ErrorParsing("Error: Cannot access php cgi");
    }
    this->_phpCgiPath = phpPath;
}

void ConfigurationParser::TEMP_CONFIGURATION::_setIndex(const STRING &index) {
    if (!this->_index.empty()) {
        throw ErrorParsing("Error: Index already set");
    }
    if (ParsingUtils::checkDoubleSlash(index) == true) {
        throw BadFile("Error: index: Double slash");
    }
    if (index.back() == '/') {
        throw ErrorParsing("Error: Index can't be a directory (end with '/')");
    }
    this->_index = index;
}

void ConfigurationParser::TEMP_CONFIGURATION::_setRoot(STRING root) {

    if (root.front() != '/') {
        throw ErrorParsing("Error: Root must start with '/'");
    }
    if (!this->_root.empty()) {
        throw ErrorParsing("Error: Root already set");
    }
    if (ParsingUtils::checkDoubleSlash(root) == true) {
        throw BadFile("Error: root: Double slash");
    }
    if (root.back() == '/') {
        root.erase(root.end() - 1);
    }
    this->_root = root;
}

void ConfigurationParser::TEMP_CONFIGURATION::_setHost(const STRING &host) {
    if (!this->_host.empty()) {
        throw ErrorParsing("Error: Host already set");
    }
    this->_host = host;
}

void ConfigurationParser::TEMP_CONFIGURATION::_setPorts(STRING &ports) {
    UINT num;

    if (!this->_ports.empty()) {
        throw ErrorParsing("Error: Ports already set");
    }
    VECTOR_STRING ports_str = ParsingUtils::split(ports, ",");
    if (ParsingUtils::checkDoubleValue(ports_str)) {
        throw ErrorParsing("Error: Double value in ports");
    }
    for (UINT i = 0; i < ports_str.size(); i++) {
        if (!ParsingUtils::betteratoi(ports_str[i].c_str(), num)) {
            throw ErrorParsing("Error: Port is not a UINTeger");
        }
        this->_ports.push_back(num);
    }
}

void ConfigurationParser::TEMP_CONFIGURATION::_setMaxClients(const STRING &maxClients) {
    UINT num;

    if (this->_maxClients_is_set) {
        throw ErrorParsing("Error: Max clients already set");
    }
    this->_maxClients_is_set = true;
    if (!ParsingUtils::betteratoi(maxClients.c_str(), num)) {
        throw ErrorParsing("Error: Port is not a UINTeger");
    }
    this->_maxClients = num;
}

void ConfigurationParser::TEMP_CONFIGURATION::_setDefaultErrorPage(const STRING &defaultErrorPage) {
    if (!this->_defaultErrorPage.empty()) {
        throw ErrorParsing("Error: Default error page already set");
    }
    if (ParsingUtils::checkDoubleSlash(defaultErrorPage) == true) {
        throw BadFile("Error: defaultErrorPages: Double slash");
    }
    if (defaultErrorPage.back() == '/') {
        throw ErrorParsing("Error: Default error page cannot be a directory (end with '/')");
    }
    this->_defaultErrorPage = defaultErrorPage;
}

void ConfigurationParser::TEMP_CONFIGURATION::_setClientBodyMaxSize(const STRING &clientBodyMaxSize) {
    UINT num;

    if (this->_clientBodyMaxSize_is_set) {
        throw ErrorParsing("Error: Client body max size already set");
    }
    this->_clientBodyMaxSize_is_set = true;
    if (!ParsingUtils::betteratoi(clientBodyMaxSize.c_str(), num)) {
        throw ErrorParsing("Error: Port is not an UINTeger");
    }
    this->_clientBodyMaxSize = num;
}

ConfigurationParser::TEMP_CONFIGURATION::_TempConfiguration() {
    this->_maxClients_is_set        = false;
    this->_clientBodyMaxSize_is_set = false;
}

ConfigurationParser::TEMP_CONFIGURATION::~_TempConfiguration() {}

STRING ConfigurationParser::TEMP_CONFIGURATION::getHost() const {
    return this->_host;
}

VECTOR_UINT ConfigurationParser::TEMP_CONFIGURATION::getPorts() const {
    return this->_ports;
}

UINT ConfigurationParser::TEMP_CONFIGURATION::getMaxClients() const {
    return this->_maxClients;
}

STRING ConfigurationParser::TEMP_CONFIGURATION::getDefaultErrorPage() const {
    return this->_defaultErrorPage;
}

UINT ConfigurationParser::TEMP_CONFIGURATION::getClientBodyMaxSize() const {
    return this->_clientBodyMaxSize;
}

STRING ConfigurationParser::TEMP_CONFIGURATION::getIndex() const {
    return this->_index;
}

STRING ConfigurationParser::TEMP_CONFIGURATION::getRoot() const {
    return this->_root;
}

STRING ConfigurationParser::TEMP_CONFIGURATION::getPhpCgiPath() const {
    return this->_phpCgiPath;
}

bool ConfigurationParser::TEMP_CONFIGURATION::checkAllFieldsSet() const {
    if (this->_phpCgiPath.empty()) {
        throw BadFile("Error: PHP CGI path not set");
    }
    if (!this->_maxClients_is_set) {
        throw BadFile("Error: Max clients not set");
    }
    if (this->_root.empty()){
        throw BadFile("Error: Root not set");
    }
    if (this->_index.empty()) {
        throw BadFile("Error: Index not set");
    }
    if (this->_host.empty()) {
        throw BadFile("Error: Host not set");
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

void ConfigurationParser::TEMP_CONFIGURATION::forceSetRoot(const STRING &root){
    this->_root = root;
}

void ConfigurationParser::TEMP_CONFIGURATION::forceSetIndex(const STRING &index){
    this->_index = index;
}

void ConfigurationParser::TEMP_CONFIGURATION::forceSetDefaultErrorPage(const STRING &defaultErrorPage){
    this->_defaultErrorPage = defaultErrorPage;
}

void ConfigurationParser::TEMP_CONFIGURATION::setFields(const STRING &field, STRING value) {

    if (field == "php_cgi"){
        this->_setPhpCgiPath(value);
    }
    else if (field == "host") {
        this->_setHost(value);
    }
    else if (field == "root") {
        this->_setRoot(value);
    } else if (field == "index") {
        this->_setIndex(value);
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
