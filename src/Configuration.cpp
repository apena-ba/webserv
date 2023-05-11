#include "Configuration.hpp"

STRING Configuration::getExtension(const STRING &path) {
        size_t i = path.find_last_of('.');
        if (i == std::string::npos) {
            return "";
        }
        STRING fileExtension = path.substr(i);
        
        return fileExtension;
}

UINT Configuration::checkPath(STRING path) const {
        VECTOR_STRING   vector;
        VECTOR_UINT     index_location_in_path;
        UINT            max_length_index;

        if (path.empty()) {
            throw ConfigurationException("Path is empty");
        }
        if (path[0] != '/') {
            throw ConfigurationException("Path must start with a '/'");
        }
        if (path.back() == '/' && path.length() > 1) {
            path.erase(path.length() - 1, 1);
        }
        for (UINT i = 0; i < routes.size(); i++) {
            std::string location = routes[i].location;
            if (location.back() == '/'){
                location.erase(location.end() - 1);
            }
            if (location.length() > path.length()) { continue; }
            if (path.find(location) == 0
                && (path[location.length()] == '/'
                    || path.length() == location.length())) {
                index_location_in_path.push_back(i);
            }
        }
        if (index_location_in_path.empty()) {
            throw ConfigurationException("Path not found");
        }
        max_length_index = index_location_in_path[0];
        for (UINT j = 0; j < index_location_in_path.size(); j++) {
            if (routes[index_location_in_path[j]].location.length() > routes[max_length_index].location.length()) {
                max_length_index = index_location_in_path[j];
            }
        }
        return max_length_index;
    }
