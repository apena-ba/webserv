#include "ConfigurationParser/utils/_TempRoute.hpp"

void TEMP_ROUTE::_setIndex(const STRING &index) {
    if (access(index.c_str(), F_OK) == -1) {
        throw ErrorParsing("Error: Cannot access file index");
    }
    if (!this->_index.empty()) {
        throw ErrorParsing("Error: Index already set");
    }
    this->_index = index;
}


void TEMP_ROUTE::_setMethods(const STRING &methods_input) {
    int             number_get      = 0;
    int             number_post     = 0;
    int             number_delete   = 0;
    VECTOR_STRING   methods         = ParsingUtils::split(
                                        methods_input, ",");

    if (!this->_methods.empty()) {
        throw ErrorParsing("Error: Methods already set");
    }
    if (methods.empty()) {
        throw ErrorParsing("Error: No method provided");
    }
    if (methods.size() > 3) {
        throw ErrorParsing("Error: Too many methods");
    }
    for (UINT i = 0; i < methods.size(); i++) {
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
        throw ErrorParsing("Error: Double value in methods");
    }
    this->_methods = ParsingUtils::toUpperVector(methods);
}

void TEMP_ROUTE::_setLocation(STRING location) {
    if (!this->_location.empty()) {
        throw ErrorParsing("Error: route: location already set");
    }
    if (location.back() == '/' && location.size() > 1) {
        location.erase(location.end() - 1);
    }
    this->_location = location;
}

TEMP_ROUTE::_TempRoute() {}

TEMP_ROUTE::~_TempRoute() {}

STRING TEMP_ROUTE::getIndex() const {
    return this->_index;
}

STRING TEMP_ROUTE::getLocation() const {
    return this->_location;
}

VECTOR_STRING TEMP_ROUTE::getMethods() const {
    return this->_methods;
}

bool TEMP_ROUTE::checkAllFieldsSet() {
    if (this->_location.empty()) {
        throw ErrorParsing("Error: Route: location is not set");
    }
    if (this->_methods.empty()) {
        throw ErrorParsing("Error: Route: methods is not set");
    }
    if (this->_index.empty()) {
        throw ErrorParsing("Error: Route: index is not set");
    }
    return true;
}

void TEMP_ROUTE::setFields(STRING &field, STRING &value) {
    if (field == "methods") {
        _setMethods(value);
    } else if (field == "location") {
        this->_setLocation(value);
    } else if (field == "index") {
        this->_setIndex(value);
    } else {
        throw ErrorParsing("Error: Invalid field");
    }
}
