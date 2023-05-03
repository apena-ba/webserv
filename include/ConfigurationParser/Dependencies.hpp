#ifndef CONF_PARSE_DEPENDENCIES
#define CONF_PARSE_DEPENDENCIES

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unistd.h>
#include "Configuration.hpp"
#include "macro.hpp"
#include "ConfigurationParser/utils/ParsingUtils.hpp"
#include "Route.hpp"

typedef ConfigurationParser::_TempRoute                             TEMP_ROUTE;

typedef std::vector<TEMP_ROUTE>                                     VECTOR_TEMP_ROUTE;

typedef ConfigurationParser::_TempConfiguration                     TEMP_CONFIGURATION;

typedef VECTOR_STRING                                               SPLITTED_FILE;

typedef std::vector<PAIR_STRING>                                    FIELDS_MODEL;

typedef std::vector<std::pair<STRING, VECTOR_STRING > >             EXTRACTED_ROUTE_MODEL;

typedef std::vector<std::pair<TEMP_CONFIGURATION, VECTOR_ROUTE > >  FINAL_MODEL;

#endif
