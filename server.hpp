#ifndef SERVER_HPP_INCLUDED
#define SERVER_HPP_INCLUDED

#include <SFML/Network.hpp>

#include "network.hpp"

class Server {
public:
    Server(uint16_t);
    ~Server();
    network::Socket Socket;
    uint16_t port;
    network::Packet Packet;

private:
};

#endif
