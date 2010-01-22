#ifndef SERVER_HPP_INCLUDED
#define SERVER_HPP_INCLUDED

#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include <boost/multi_array.hpp>

#include <string>

#include "network.hpp"

typedef boost::multi_array<std::string, 2> stringarray2d;
typedef boost::multi_array<sf::Uint16, 2> uint16array2d;

// Main class for server application.
class ServerApp {
public:
    ServerApp(const uint16_t&, const uint16_t&);
    ~ServerApp();

    // Initializes server.
    void Init();

    // Listens for incoming client requests and acts accordingly.
    // This method takes care of client registering and data gathering.
	void HandleRequest();

	// Data gathered by HandleRequest() is evaluated here and sent back to
	// clients.
	void Update();

	// Safely shuts down the server.
	void Die();

    // Get server socket.
	Network::Socket& GetSocket();

    // Get server packet for receiving client data.
	Network::Packet& GetRecvPacket();

	// Get server packet for sending client data.
	Network::Packet& GetSendPacket();

	// Get
	sf::Uint8 GetPlayerId(std::string);
private:
    uint16_t m_port;
    uint16_t m_maxplayers;
    uint16_t m_clientport;
	stringarray2d m_clist;
	stringarray2d::extent_gen extents;
    uint16array2d m_mpos;
    uint16array2d m_blocks;
    Network::Socket Socket;
    Network::Packet RecvPacket;
    Network::Packet SendPacket;
};

#endif
