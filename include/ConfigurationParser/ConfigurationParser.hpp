#ifndef CONFIGURATION_PARSER
#define CONFIGURATION_PARSER

#include "Dependencies.hpp"

class ConfigurationParser {
private:
    class                           _TempConfiguration;

    class                           _TempRoute;

    static  PAIR_STRING              _lineToPair(
                                        STRING line);

    static  Configuration           _toConfiguration(
                                        TEMP_CONFIGURATION &server,
                                        const VECTOR_ROUTE &routes);

    static  VECTOR_CONFIGURATION    _modelToConfiguration(
                                        FINAL_MODEL model);

    FIELDS_MODEL                    _fieldExtractor(
                                        const STRING &line,
                                        const STRING &opener);

    static  bool                    _checkDoubleRoute(
                                        VECTOR_ROUTE &routes);

    static VECTOR_ROUTE             _tmpToRoute(
                                        VECTOR_TEMP_ROUTE data);

    VECTOR_ROUTE                    _dataToRoute(
                                        VECTOR_STRING data);

    TEMP_CONFIGURATION              _dataToConfiguration(
                                        const STRING &data);

    FINAL_MODEL                     _dataToModel(
                                        EXTRACTED_ROUTE_MODEL data);

    static EXTRACTED_ROUTE_MODEL    _extractRoute(
                                        SPLITTED_FILE servers);

    static UINT                     _findCloseBrace
                                        (STRING str);

    static SPLITTED_FILE            _serverSplitter(
                                        const STRING &file);

public:
    ConfigurationParser();

    ~ConfigurationParser();

    VECTOR_CONFIGURATION            parse(
                                        const STRING &path);

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
