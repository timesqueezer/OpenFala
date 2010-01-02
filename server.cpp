#include <SFML/System.hpp>
#include <iostream>

#include "server.hpp"

Server::Server(uint16_t port) {
    Socket.Bind(port);
}

Server::~Server() {}

