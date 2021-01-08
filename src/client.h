#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>


class Client
{
public:
    static const size_t MAX_RESPONSE_LENGTH = 1 << 14;

    Client(std::string hostname, const size_t portno)
        : hostname_(std::move(hostname)), portno_(portno) {}

    void setupThis();

    std::string get(const std::string &req);

private:
    std::string hostname_;
    const size_t portno_;
    int sockfd_{};
    sockaddr_in serv_addr_{};
    hostent *server_;
};

#endif // CLIENT_H
