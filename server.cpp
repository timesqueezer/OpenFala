#include <SFML/System.hpp>
#include <iostream>

#include "server.hpp"

Server::Server(const uint16_t& port) {
    socket.Bind(port);
    m_port = port;
}

Server::~Server() {}

/*Server::Handle_Request() {

 	sf::Uint16 mx = 0;
 	sf::Uint16 my = 0;

 	server.getSocket().Receive(server.getPacket(), server.m_claddress, server.m_port);
    server.getPacket() >> mx >> my;
    std::cout << "MouseX: " << mx << " MouseY: " << my << std::endl;
    server.getPacket().Clear();
}
*/
network::Socket& Server::getSocket() {
	return socket;
}

network::Packet& Server::getPacket() {
	return packet;
}
