#ifndef TEMP_ROUTE_HPP
#define TEMP_ROUTE_HPP

#include "../ConfigurationParser.hpp"
#include "macro.hpp"

class ConfigurationParser::_TempRoute {
private:
    STRING          _index;

    VECTOR_STRING   _methods;

    STRING          _location;

    void            _setIndex(const STRING &index);

    void            _setMethods(const STRING &methods_input);

    void            _setLocation(STRING location);

public:
    _TempRoute();

    ~_TempRoute();

    STRING          getIndex() const;

    STRING          getLocation() const;

    VECTOR_STRING   getMethods() const;

    bool            checkAllFieldsSet();

    void            forceSetLocation(STRING location);

    void            forceSetIndex(STRING index);

    void            setFields(STRING &field, STRING &value);

    class ErrorParsing : public std::exception {
    private:
        const char *_msg;
    public:
        ErrorParsing(const char *msg) : _msg(msg) {};

        virtual const char *what() const throw() {
            return (this->_msg);
        }
    };
};



#endif
