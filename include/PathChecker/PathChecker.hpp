#ifndef PATH_CHECKER
#define PATH_CHECKER

#include <iostream>
#include <vector>
#include <fstream>
#include "../Configuration.hpp"
#include "../ConfigurationParser/utils/ParsingUtils.hpp"

class PathChecker {
private:
    PathChecker();

    std::vector<std::string> _path_vector;
    std::string _path;

    const std::string _set_path(std::string &path) {
        if (path.empty()) {
            throw PathCheckerException("Path is empty");
        }
        if (path[0] != '/') {
            throw PathCheckerException("Path must start with '/'");
        }
        return path;
    }

public:
    ~PathChecker() {}

    PathChecker(std::string &path) : _path(_set_path(path)), _path_vector(ParsingUtils::split(path, "/")) {}


    static int checkPath(std::string &path) {

    }

    class PathCheckerException : public std::exception {
    private:
        const char *_msg;
    public:
        PathCheckerException(const char *msg) : _msg(msg) {};

        virtual const char *what() const throw() {
            return (this->_msg);
        };
    };
};

#endif