#include "Configuration.hpp"

STRING Configuration::getExtension(const STRING &path) {
        STRING fileExtension = path.substr(path.find_last_of('.'));
        
        if (fileExtension == ".py") {
            return ".py";
        }
        if (fileExtension == ".php") {
            return ".php";
        }
        if (fileExtension == ".html") {
            return ".html";
        }
        return "";
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
            if (routes[i].location.length() > path.length()) { continue; }
            if (path.find(routes[i].location) == 0
                && (path[routes[i].location.length()] == '/'
                    || path.length() == routes[i].location.length())) {
                index_location_in_path.push_back(i);
            }
        }
        if (index_location_in_path.empty()) {
            for (UINT i = 0; i < routes.size(); i++) {
                if (routes[i].location == "/") {
                    return (i);
                }
            }
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