#ifndef SERVER_HPP_INCLUDED
#define SERVER_HPP_INCLUDED

#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "network.hpp"

class Server {
public:
    Server(const uint16_t&);
    ~Server();
    uint16_t m_port;
	network::IPAddress m_claddress;
	network::Socket& getSocket();
	network::Packet& getPacket();
	//Handle_Request();

private:
    network::Socket socket;
    network::Packet packet;

};

#endif
