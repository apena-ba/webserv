#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include "../../webservfinaltest/include/HTTPRequestParser.hpp"
#include "../../webservfinaltest/include/Configuration.hpp"

class Cgi{
private:
    static std::string                  _findScript(const std::string & path);
    static std::vector<const char *>    _fillEnv(HTTPRequestParser &request, const Configuration & config);
public:
    static std::string process(const HTTPRequestParser &request, const Configuration & config, std::string stdin_content);
};

#endif
