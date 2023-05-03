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

#define TEMP_ROUTE              ConfigurationParser::_TempRoute

#define VECTOR_TEMP_ROUTE       std::vector<TEMP_ROUTE>

#define TEMP_CONFIGURATION      ConfigurationParser::_TempConfiguration

#define SPLITTED_FILE           VECTOR_STRING

#define FIELDS_MODEL            std::vector<PAIR_STRING>

#define EXTRACTED_ROUTE_MODEL   std::vector<std::pair<STRING,
                                    VECTOR_STRING > >

#define FINAL_MODEL             std::vector<std::pair<
                                    TEMP_CONFIGURATION,
                                    VECTOR_ROUTE > >

#endif
