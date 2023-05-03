#ifndef TEMP_CONFIGURATION_HPP
#define TEMP_CONFIGURATION_HPP

#include "../ConfigurationParser.hpp"
#include "macro.hpp"

class ConfigurationParser::_TempConfiguration {
private:
    unsigned int                _maxClients;
    bool                        _maxClients_is_set;
    STRING                      _defaultErrorPage;
    unsigned int                _clientBodyMaxSize;
    bool                        _clientBodyMaxSize_is_set;
    std::vector<unsigned int>   _ports;
    STRING                      _host;

    void                        _setHost(const STRING &host);

    void                        _setPorts(STRING &ports);

    void                        _setMaxClients(const STRING
                                    &maxClients);

    void                        _setDefaultErrorPage(
                                    const STRING
                                    &defaultErrorPage);

    void                        _setClientBodyMaxSize(
                                    const STRING
                                    &clientBodyMaxSize);

public:
    _TempConfiguration();

    ~_TempConfiguration();

    STRING                      getHost() const;

    std::vector<unsigned int>   getPorts() const;

    unsigned int                getMaxClients() const;

    STRING                      getDefaultErrorPage() const;

    unsigned int                getClientBodyMaxSize() const;

    bool                        checkAllFieldsSet() const;

    void                        setFields(const STRING
                                            &field, STRING value);

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
