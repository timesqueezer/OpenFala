#ifndef SERVER_HPP_INCLUDED
#define SERVER_HPP_INCLUDED

#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include <boost/multi_array.hpp>

#include <string>

#include "network.hpp"

typedef boost::multi_array<std::string, 2> stringarray2d;
typedef boost::multi_array<sf::Uint16, 2> uint16array2d;


class ServerApp {
public:
    ServerApp(const uint16_t&, const uint16_t&);
    ~ServerApp();
	Network::Socket& GetSocket();
	Network::Packet& GetPacket();
	Network::Packet& GetSendPacket();

	void HandleRequest();
	void Update();
	sf::Uint8 GetPlayerId(std::string);

private:
    uint16_t m_port;
    uint16_t m_maxplayers;
	stringarray2d m_clist;
	stringarray2d::extent_gen extents;
    uint16array2d m_mpos;
    Network::Socket Socket;
    Network::Packet Packet;
    Network::Packet SendPacket;
};

#endif
