#ifndef R0UTE_HPP
#define R0UTE_HPP

#include <iostream>
#include "ConfigurationParser/utils/ParsingUtils.hpp"

class Route {
private:
public:
    Route(const std::string &_index,
          const std::vector<std::string> &_methods,
          const std::string &_location, const std::string & _redirection) : redirection(_redirection), index(_index), methods(_methods),
            location(_location) {}

    Route(const Route &route) : redirection(route.redirection), index(route.index),
                                methods(route.methods), location(route.location) {}

    Route() : index(), methods(), location() {}

    ~Route() {};

    Route(Route &route) : redirection(route.redirection), index(route.index),  methods(route.methods), location(route.location){}

    const std::string redirection;
    const std::string index;
    const std::vector<std::string> methods;
    const std::string location;
};
typedef std::vector <Route>         VECTOR_ROUTE;
#endif
