#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include "HTTPRequestParser.hpp"
#include "Configuration.hpp"

class Cgi{
private:
public:
    static std::string process(const HTTPRequestParser &request, const Configuration & config, std::string stdin_content);
};

#endif
