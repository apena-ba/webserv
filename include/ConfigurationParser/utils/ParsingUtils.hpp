/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingUtils.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efournou <efournou@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 20:16:41 by efournou          #+#    #+#             */
/*   Updated: 2023/05/16 20:16:41 by efournou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_UTILS_HPP
#define PARSING_UTILS_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include "../../macro.hpp"

class ParsingUtils {
public:
    ParsingUtils();

    ~ParsingUtils();

    static  bool                checkDoubleValue(VECTOR_STRING vector);

    static  STRING              toUpper(STRING str);

    static  bool                isNumber(STRING str);

    static  bool                betteratoi(const char *str, UINT &num);

    static  VECTOR_STRING       toUpperVector(VECTOR_STRING vec);

    static  STRING              removeIsSpace(STRING str);

    static  VECTOR_STRING       split(STRING str, const STRING &delimiter);

    static  bool                checkLimiter(STRING str);

    static  STRING              fileToString(const STRING &filePath);

    static  bool                checkDoubleSlash(const STRING & str);

    class ErrorParsing : public std::exception {
    private:
        const char *_msg;
    public:
        ErrorParsing(const char *msg) : _msg(msg) {};

        virtual const char *what() const throw() {
            return (this->_msg);
        };
    };
};

#endif
