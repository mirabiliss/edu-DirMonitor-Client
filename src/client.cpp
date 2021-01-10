#include "client.h"

void Client::setupThis()
{
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_ < 0)
        throw std::runtime_error("ERROR opening socket");

    server_ = gethostbyname(hostname_.c_str());
    if (!server_)
        throw std::runtime_error("ERROR getting server by name");


    serv_addr_.sin_family = AF_INET;
    bcopy(server_->h_addr,
         (char *)&serv_addr_.sin_addr.s_addr,
         server_->h_length);
    serv_addr_.sin_port = htons(portno_);

    if (connect(sockfd_, (struct sockaddr *) &serv_addr_, sizeof(serv_addr_)) < 0)
        throw std::runtime_error("ERROR connecting to server");
}

std::string Client::get(const std::string &req)
{
    int n = write(sockfd_, req.c_str(), req.length());
    if (n < 0)
        throw std::runtime_error("ERROR writing to socket");

    char *buffer = new char[MAX_RESPONSE_LENGTH];

    n = read(sockfd_, buffer, MAX_RESPONSE_LENGTH);
    if (n < 0)
        throw std::runtime_error("ERROR reading from socket");

    auto res = std::string(buffer);
    free(buffer);

    return res;
}
