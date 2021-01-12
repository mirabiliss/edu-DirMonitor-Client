#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

class Client
{
public:
    static const size_t MAX_RESPONSE_LENGTH = 1 << 24;

    Client(std::string hostname, const size_t portno)
        : hostname_(std::move(hostname)), portno_(portno) {
        logger = spdlog::get("client_logger");
    }

    void setupThis();

    std::string get(const std::string &req);

private:
    std::string hostname_;
    const size_t portno_;
    int sockfd_{};
    sockaddr_in serv_addr_{};
    hostent *server_;

    std::shared_ptr<spdlog::logger> logger;
};

#endif // CLIENT_H
