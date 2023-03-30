#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "macro.hpp"
#include <vector>

class Clients
{
    private:
    std::vector<pollfd> _fds;
    std::vector<std::string> _requests;        
    
    public:
        void    push(int fd, short event);
        bool    remove(unsigned int index);
        bool    setEvent(unsigned int index, short event);
        pollfd * pollData(void);
        nfds_t size(void);
        std::string * getContentByIndex(unsigned int index);
        bool pushContentToIndex(unsigned int index, char * content);
        pollfd * getPollFdByIndex(unsigned int index);
        Clients(){};
        ~Clients(){};
};

#endif