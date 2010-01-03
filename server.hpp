#ifndef SERVER_HPP_INCLUDED
#define SERVER_HPP_INCLUDED

#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include <boost/multi_array.hpp>

#include <string>

#include "network.hpp"

typedef boost::multi_array<std::string, 2> stringarray2d;

class ServerApp {
public:
    ServerApp(const uint16_t&, const uint16_t&);
    ~ServerApp();
	Network::Socket& GetSocket();
	Network::Packet& GetPacket();

	void HandleRequest();
	void Update();

private:
    uint16_t m_port;
    uint16_t m_maxplayers;
	stringarray2d m_clist;
	stringarray2d::extent_gen extents;

    Network::Socket Socket;
    Network::Packet Packet;
};

#endif
