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
            throw std::runtime_error("Could not open socket.");
        }

        logger->info("Socket has been succesfully opened.");
        logger->debug("Getting server by name '{}'.", hostname_);

        server_ = gethostbyname(hostname_.c_str());
        if (!server_) {
            throw std::runtime_error("Could not determine server name.");
        }

        logger->info("Server has been found; name: {}", hostname_);

        serv_addr_.sin_family = AF_INET;
        bcopy(server_->h_addr,
             (char *)&serv_addr_.sin_addr.s_addr,
             server_->h_length);
        serv_addr_.sin_port = htons(portno_);

        logger->debug("Connecting to the server...");

        if (connect(sockfd_, (struct sockaddr *) &serv_addr_, sizeof(serv_addr_)) < 0) {
            throw std::runtime_error(fmt::format("Could not connect to the server {}:{}", hostname_, portno_));
        }

        logger->info("Client has been connected to the server successfully.");

    } catch(const std::runtime_error &error) {
        logger->error("{}", error.what());
        __throw_exception_again;
    }

    // checking server response for connection
    auto response = this->recv();
    try {
        auto responseJson = json::parse(response);
        if (!responseJson["status"].get<bool>()) {
            std::string errorText = responseJson["reason"].get<std::string>();
            throw std::runtime_error(errorText);
        }
    } catch (const json::parse_error &e) {
        std::cout << e.what();
        __throw_exception_again;
    } catch (const std::runtime_error &e) {
        __throw_exception_again;
    }
}

void Client::send(const std::string &req)
{
    logger->debug("Writing to the socket...");

    int n = write(sockfd_, req.c_str(), req.length());
    if (n < 0)
        throw std::runtime_error("Error writing to socket.");

    logger->info("Request has been written to the socket.");
}

std::string Client::recv()
{
    logger->debug("Reading from socket...");

    uint32_t readBytes = 0;
    uint32_t datalen;
    if (read(sockfd_, &datalen, sizeof(datalen)) < 0)
        throw std::runtime_error("Could not read length of retrieving data.");
    datalen = ntohl(datalen);

    char *buffer = new char[datalen];
    if (!buffer) {
        throw std::runtime_error("Memory for string buffer was not allocated.");
    }
    bzero(buffer, datalen);
    char *buffy = new char[CHUNK_SIZE];
    while (readBytes < datalen) {
        int n = read(sockfd_, buffy, CHUNK_SIZE);
        if (n < 0) break;
        readBytes += n;

        strcat(buffer, buffy);
        bzero(buffy, CHUNK_SIZE);
    }

    logger->info("Response has been read from the socket.");

    std::string res;
    res.assign(buffer, datalen);
    return res;
}

std::string Client::get(const std::string &req)
{
    send(req);
    auto res = recv();

    return res;
}
