#ifndef CONFIGURATION_PARSER
#define CONFIGURATION_PARSER

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unistd.h>
#include "macro.hpp"
#include "Configuration.hpp"
#include "Route.hpp"

class ConfigurationParser {
private:
    // Class that I use to store the data of the configuration file and check it
    class                           _TempConfiguration;

    class                           _TempRoute;

    // Types
    typedef _TempRoute                                                  TEMP_ROUTE;

    typedef std::vector<TEMP_ROUTE>                                     VECTOR_TEMP_ROUTE;

    typedef _TempConfiguration                                          TEMP_CONFIGURATION;

    typedef VECTOR_STRING                                               SPLITTED_FILE;

    typedef std::vector<PAIR_STRING>                                    FIELDS_MODEL;

    typedef std::vector<std::pair<STRING, VECTOR_STRING > >             EXTRACTED_ROUTE_MODEL;

    typedef std::vector<std::pair<TEMP_CONFIGURATION, VECTOR_ROUTE > >  FINAL_MODEL;

    // Methods
    static  PAIR_STRING              _lineToPair(STRING line);

    static  Configuration           _toConfiguration(TEMP_CONFIGURATION &server, const VECTOR_ROUTE &routes);

    static  VECTOR_CONFIG           _modelToConfiguration(FINAL_MODEL model);

    FIELDS_MODEL                    _fieldExtractor(const STRING &line, const STRING &opener);

    static  bool                    _checkDoubleRoute(VECTOR_ROUTE &routes);

    static VECTOR_ROUTE             _tmpToRoute(VECTOR_TEMP_ROUTE data);

    VECTOR_ROUTE                    _dataToRoute(VECTOR_STRING data);

    TEMP_CONFIGURATION              _dataToConfiguration(const STRING &data);

    FINAL_MODEL                     _dataToModel(EXTRACTED_ROUTE_MODEL data);

    static EXTRACTED_ROUTE_MODEL    _extractRoute(SPLITTED_FILE servers);

    static UINT                     _findCloseBrace(STRING str);

    static SPLITTED_FILE            _serverSplitter(const STRING &file);

public:
    ConfigurationParser();

    ~ConfigurationParser();


    VECTOR_CONFIG            parse(const STRING &path);

    class BadFile : public std::exception {
    private:
        const char *_msg;
    public:
        BadFile(const char *msg) : _msg(msg) {};

        virtual const char *what() const throw() {
            return (this->_msg);
        };
    };
};

#endif
