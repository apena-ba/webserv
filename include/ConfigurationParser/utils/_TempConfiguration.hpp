#ifndef TEMP_CONFIGURATION_HPP
#define TEMP_CONFIGURATION_HPP

#include "../ConfigurationParser.hpp"
#include "macro.hpp"

class ConfigurationParser::_TempConfiguration {
private:
    UINT                        _maxClients;
    bool                        _maxClients_is_set;
    UINT                        _clientBodyMaxSize;
    bool                        _clientBodyMaxSize_is_set;
    STRING                      _defaultErrorPage;
    VECTOR_UINT                 _ports;
    STRING                      _host;
    STRING                      _root;
    STRING                      _index;
    STRING                      _CgiPath;
    STRING                      _cgiExtension;

    void                        _setCgiExtension(const STRING &cgiExtension);

    void                        _setCgiPath(const STRING &Path);

    void                        _setRoot(STRING root);

    void                        _setIndex(const STRING &index);

    void                        _setHost(const STRING &host);

    void                        _setPorts(STRING &ports);

    void                        _setMaxClients(const STRING &maxClients);

    void                        _setDefaultErrorPage(const STRING &defaultErrorPage);

    void                        _setClientBodyMaxSize(const STRING &clientBodyMaxSize);

public:
    _TempConfiguration();

    ~_TempConfiguration();

    STRING                      getCgiExtension() const;

    STRING                      getCgiPath() const;

    STRING                      getIndex() const;

    STRING                      getRoot() const;

    STRING                      getHost() const;

    VECTOR_UINT                 getPorts() const;

    UINT                        getMaxClients() const;

    STRING                      getDefaultErrorPage() const;

    UINT                        getClientBodyMaxSize() const;

    bool                        checkAllFieldsSet() const;

    void                        forceSetRoot(const STRING &root);

    void                        forceSetIndex(const STRING &index);

    void                        forceSetDefaultErrorPage(const STRING &defaultErrorPage);

    void                        setFields(const STRING &field, STRING value);

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
