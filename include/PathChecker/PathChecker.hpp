#ifndef PATH_CHECKER
#define PATH_CHECKER

#include <iostream>
#include <vector>
#include <fstream>
#include "../Configuration.hpp"
#include "../ConfigurationParser/ParsingUtils.hpp"

class PathChecker {
private:
    PathChecker();

    std::vector<std::string> _path;
public:
    ~PathChecker() {}

    PathChecker(std::string &path) {
        if (path.empty()) {
            throw PathCheckerException("Path is empty");
        }
        if (path[0] != '/') {
            throw PathCheckerException("Path must start with '/'");
        }
        this->_path = ParsingUtils::split(path, "/");
    }

    static std::string getExtension(std::string &path) {

    }

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