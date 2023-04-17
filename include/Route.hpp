#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <iostream>

class Route
{
    private:
        std::string _path;
        std::vector <std::string> _methods;
        std::string _location;
        std::string _index;
        std::bool _directory_listing;
        std::string cgi;
    public:
        Route();
        ~Route();
    
    

};

#endif