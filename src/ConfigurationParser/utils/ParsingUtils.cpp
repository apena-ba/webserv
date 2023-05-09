#include "ConfigurationParser/utils/ParsingUtils.hpp"

ParsingUtils::ParsingUtils() {}

ParsingUtils::~ParsingUtils() {}

bool ParsingUtils::checkDoubleValue(VECTOR_STRING vector) {
    for (UINT i = 0; i < vector.size(); i++) {
        for (UINT j = i + 1; j < vector.size(); j++) {
            if (vector[i] == vector[j]) {
                return true;
            }
        }
    }
    return false;
}

STRING ParsingUtils::toUpper(STRING str) {
    for (UINT i = 0; i < str.size(); i++) {
        str[i] = std::toupper(str[i]);
    }
    return str;
}

bool ParsingUtils::isNumber(STRING str) {
    for (UINT i = 0; i < str.size(); i++) {
        if (!std::isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

bool ParsingUtils::betteratoi(const char *str, UINT &num) {
    long long int tmp;
    if (!isNumber(str)) {
        return false;
    }
    if (std::strlen(str) > 10) {
        return false;
    }
    tmp = 0;
    for (uint32_t i = 0; str[i]; i++) {
        tmp *= 10;
        tmp += str[i] - '0';
        if (tmp > (long long int) std::numeric_limits<UINT>::max()) {
            return false;
        }
    }
    if (tmp <= 0) {
        return false;
    }
    num = static_cast<int>(tmp);
    return true;
}

VECTOR_STRING ParsingUtils::toUpperVector(VECTOR_STRING vec) {
    for (UINT i = 0; i < vec.size(); i++) {
        vec[i] = toUpper(vec[i]);
    }
    return vec;
}

STRING ParsingUtils::removeIsSpace(STRING str) {
    STRING ret;

    for (UINT i = 0; i < str.size(); i++) {
        if (!(std::isspace(str[i]))) {
            ret += str[i];
        }
    }
    return ret;
}

VECTOR_STRING ParsingUtils::split(STRING str, const STRING &delimiter) {
    VECTOR_STRING   result;
    size_t          pos;
    STRING          token;

    while ((pos = str.find(delimiter)) != STRING::npos) {
        token = str.substr(0, pos);
        result.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    if (!str.empty()) { result.push_back(str); }
    return result;
}

bool ParsingUtils::checkLimiter(STRING str) {
    int openLimiterNumber   = 0;
    int closeLimiterNumber  = 0;

    for (UINT i = 0; i < str.size(); i++) {
        if (str[i] == '{') {
            openLimiterNumber++;
        } else if (str[i] == '}') {
            closeLimiterNumber++;
            if (closeLimiterNumber > openLimiterNumber) {
                throw ErrorParsing("Error: Bad server format: More close limiter than open limiter");
            }
        }
    }
    return true;
}

bool ParsingUtils::checkDoubleSlash(const STRING & str){
    for (UINT i = 0; i < str.size(); i++) {
        if (str[i] == '/' && str[i + 1] == '/') {
            return true;
        }
    }
    return false;
}


STRING ParsingUtils::fileToString(const STRING &filePath) {
    std::ifstream file(filePath);

    STRING str((std::istreambuf_iterator<char>(file)),
               std::istreambuf_iterator<char>());
    return str;
}
