#include "../include/Clients.hpp"

void Clients::push(int fd, short event)
{
    std::string tempStr = "test";
    pollfd temp;
    temp.fd = fd;
    temp.events = event;
    temp.revents = 0;
    this->_fds.push_back(temp);
    this->_requests.push_back(tempStr);
}

nfds_t Clients::size(void)
{
    return this->_fds.size();
}

bool Clients::remove(unsigned int index)
{
    if (this->_fds.size() <= index
        || this->_requests.size() <= index)
    {
        return false;
    }
    this->_fds.erase(this->_fds.begin() + index);
    this->_requests.erase(this->_requests.begin() + index);
    return true;
}

pollfd * Clients::pollData(void)
{
    return (this->_fds.data());
}

bool    Clients::setEvent(unsigned int index, short event)
{
    if (this->_fds.size() <= index) { return false; }
    this->_fds[index].events |= event;
    return true;
}

std::string * Clients::getContentByIndex(unsigned int index)
{
    if (this->_requests.size() <= index)
    {
        return NULL;
    }
    return &this->_requests[index];
}

pollfd * Clients::getPollFdByIndex(unsigned int index)
{
    if (this->_fds.size() <= index)
    {
        return NULL;
    }
    return &this->_fds[index];
}

bool Clients::pushContentToIndex(unsigned int index, char * content)
{
    if (this->_fds.size() <= index)
    {
        return false;
    }
    this->_requests[index].append(content);
    return true;
}


