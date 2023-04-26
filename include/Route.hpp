#ifndef R0UTE_HPP
#define R0UTE_HPP

#include <iostream>

class Route {
private:


public:
    Route(const std::string &_index,
          const std::vector<std::string> &_methods,
          const std::string &_path) : index(_index), methods(_methods),
                                      path(_path) {}

    Route(const Route &route) : index(route.index), methods(route.methods), path(route.path) {
    }

    Route() : index(), methods(), path() {}

    ~Route() {};

    Route(Route &route) : index(route.index), methods(route.methods), path(route.path) {}

    const std::string index;
    const std::vector<std::string> methods;
    const std::string path;
};

#endif