#include "client.h"
// /home/oleh/edu-DirMonitor
void Client::setupThis()
{
    //auto logger = spdlog::get("client_logger");

    try {
    logger->debug("Openning socket...");
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_ < 0)
    {  
        throw std::string("Cannot open socket");
    }

    logger->info("Socket has been succesfully openned");

    logger->debug("Getting server by name");

    server_ = gethostbyname(hostname_.c_str());
    if (!server_) {
        throw std::string("Cannot get server by name");
    }

    logger->info("Server has been found; name: {}", hostname_);

    serv_addr_.sin_family = AF_INET;
    bcopy(server_->h_addr,
         (char *)&serv_addr_.sin_addr.s_addr,
         server_->h_length);
    serv_addr_.sin_port = htons(portno_);

    logger->debug("Connecting to the server");

    if (connect(sockfd_, (struct sockaddr *) &serv_addr_, sizeof(serv_addr_)) < 0) {
        throw std::string("Cannot connect to server");
    }

    logger->info("Client has been connected to the server");

    } catch(std::string error) {
        logger->error("{}", error);
        if(error == "Cannot connect to server") {
            logger->debug("Check port");
        }
    }

}

std::string Client::get(const std::string &req)
{
    logger->debug("Writing to socket...");

    int n = write(sockfd_, req.c_str(), req.length());
    if (n < 0)
        throw std::runtime_error("ERROR writing to socket");

    logger->info("Request has been written to socket");

    char *buffer = new char[MAX_RESPONSE_LENGTH];

    logger->debug("Reading from socket...");

    n = read(sockfd_, buffer, MAX_RESPONSE_LENGTH);
    if (n < 0)
        throw std::runtime_error("ERROR reading from socket");

    logger->info("Response has been read from socket");

    auto res = std::string(buffer);
    free(buffer);

    return res;
}
