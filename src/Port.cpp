/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Port.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 19:47:14 by apena-ba          #+#    #+#             */
/*   Updated: 2023/05/16 19:43:23 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Port.hpp"

Port::Port(unsigned int portNumber, std::vector<Server*> servers) : _timeOutRead(initializeTimeOutRead()), _timeOutWrite(initializeTimeOutWrite())
{
    this->_port = portNumber;
    this->_portFd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_portFd < 1)
        throw (Port::FailSocketDeclarationException());
    std::cout << "Port number " << portNumber << " is on" << std::endl;
    if(this->setSockTimeOut(this->_portFd) == false)
        throw (Port::FailTimeOutException());
    this->_addressLen = sizeof(this->_address);
    this->_address.sin_family = AF_INET;
    this->_address.sin_addr.s_addr = INADDR_ANY;
    this->_address.sin_port = htons(portNumber);
    
    std::memset(this->_address.sin_zero, 0, this->_addressLen);
    if(bind(this->_portFd, (struct sockaddr *)&this->_address, sizeof(this->_address)) == -1)
        throw (Port::FailBindException());
    if(listen(this->_portFd, MAXCLIENT) == -1)
        throw (Port::FailListenException());

    pollfd port_pollfd;
    port_pollfd.fd = this->_portFd;
    port_pollfd.events = POLLIN | POLLERR | POLLHUP;
    port_pollfd.revents = 0;
    this->_pollFds.push_back(port_pollfd);
    this->_requests.push_back("");
    this->_servers = servers;
}

Port::Port() : _timeOutRead(initializeTimeOutRead()), _timeOutWrite(initializeTimeOutWrite()){}

unsigned int Port::getPollfdsSize(void) const
{
    return this->_pollFds.size();
}

Port::Port(Port const &to_copy) : _timeOutRead(initializeTimeOutRead()), _timeOutWrite(initializeTimeOutWrite())
{
    this->_port = to_copy._port;
    this->_portFd = to_copy._portFd;
    this->_pollFds = to_copy._pollFds;
    this->_requests = to_copy._requests;
    this->_servers = to_copy._servers;
    this->_address = to_copy._address;
    this->_addressLen = to_copy._addressLen;
}

Port::~Port(){
}

// METHODS

void Port::updateFds(std::vector<pollfd> &general_fds, unsigned int *index)
{
    for(unsigned int i = 0; i < this->_pollFds.size(); i++)
    {
        this->_pollFds[i] = general_fds[*index];
        *index += 1;
    }
}

void Port::closeClient(unsigned int index)
{
    close(this->_pollFds[index].fd);
    this->_pollFds.erase(this->_pollFds.begin() + index);
    this->_requests.erase(this->_requests.begin() + index);
}

unsigned int Port::getPort(void) const
{
    return this->_port;
}


bool Port::setSockTimeOut(int fd)
{
    if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const void *) &this->_timeOutRead, sizeof(this->_timeOutRead)) < 0)
        return (false);
    if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (const void *) &this->_timeOutWrite, sizeof(this->_timeOutWrite)) < 0)
        return (false);
    return (true);
}

void Port::createNewClient(void)
{
    int                 new_client;
    struct sockaddr_in  client_address;
    socklen_t           client_address_length = sizeof(client_address);

    new_client = accept(this->_pollFds[0].fd, (struct sockaddr *)&client_address, &client_address_length);
    if (new_client > 0 && this->setSockTimeOut(new_client) == true && fcntl(new_client, F_SETFL, O_NONBLOCK) == 0)
    {
        pollfd client_pollfd;
        client_pollfd.fd = new_client;
        client_pollfd.events = POLLIN | POLLERR | POLLHUP;
        client_pollfd.revents = 0;
        this->_pollFds.push_back(client_pollfd);
        this->_requests.push_back("");
    }
}

pollfd Port::getPollfdByIndex(int index)
{
    return this->_pollFds[index];
}

void Port::selectServer(unsigned int i)
{
    int anywhere_index = -1;
    for(unsigned int x = 0; x < this->_servers.size(); x++)
    {
        HTTPRequestParser parser(this->_requests[i]);
        if(this->_servers[x]->getHost() == "0.0.0.0")
            anywhere_index = i;
        if(parser.get("host") == this->_servers[x]->getHost())
        {
            if(this->_servers[x]->handleRequest(parser, this->_pollFds[i].fd) == true){
                std::cout << "vec size = " << this->_pollFds.size() << std::endl;
                this->closeClient(i);
            }
            return;
        }
    }
    if(anywhere_index != -1)
    {
        HTTPRequestParser parser(this->_requests[i]);
        if(this->_servers[anywhere_index]->handleRequest(parser, this->_pollFds[i].fd) == true)
            this->closeClient(i);
        return;
    }
    this->closeClient(i); // This only happens when no server host is available, maybe we shuld send a status code?
}

void Port::run(void)
{
    int     ret_read;
    char    buff[BUFFER_SIZE + 1];

    if(this->_pollFds[0].revents & POLLIN)
        this->createNewClient();
    memset(buff, 0, BUFFER_SIZE + 1);
    for(unsigned int i = 1; i < this->_pollFds.size(); i++)
    {
        if(this->_pollFds[i].fd > 0 && this->_pollFds[i].revents & POLLIN) // Check reading
        {
            ret_read = read(this->_pollFds[i].fd, buff, BUFFER_SIZE);
            this->_pollFds[i].events |= POLLOUT;
            if (ret_read < BUFFER_SIZE && ret_read > 0)
                this->_requests[i].append(buff);
            else if (ret_read > 0)
                this->_requests[i].append(buff);
        }
        else if(this->_pollFds[i].fd > 0 && this->_pollFds[i].revents & POLLOUT && !(this->_pollFds[i].revents & POLLIN)) // Check writing
            this->selectServer(i);
        else if(this->_pollFds[i].fd > 0 && (this->_pollFds[i].revents & POLLHUP || this->_pollFds[i].revents & POLLERR)) // Check error
            this->closeClient(i);
    }
}
