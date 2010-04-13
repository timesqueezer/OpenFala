#ifndef SERVER_HPP_INCLUDED
#define SERVER_HPP_INCLUDED

#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include <boost/multi_array.hpp>

#include <string>

//#include <ClientManager.hpp>

typedef boost::multi_array<std::string, 2> stringarray2d;
typedef boost::multi_array<sf::Uint16, 2> uint16array2d;

// Main class for server application.
class ServerApp {
public:
    ServerApp(const sf::Uint16&, const sf::Uint16&);
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
	sf::SocketUDP& GetSocket();

	// Get server packet for sending client data.
	sf::Packet& GetSendPacket();

	// Get
	int GetPlayerId(std::string);
private:
    sf::Uint8 m_active_clients;
    sf::Uint16 m_port;
    sf::Uint16 m_maxplayers;
    sf::Uint16 m_clientport;
	stringarray2d m_clist;
	stringarray2d::extent_gen extents;
    uint16array2d m_mpos;
    uint16array2d m_blocks;
    sf::SelectorUDP Selector;
    sf::SocketUDP Listener;
    sf::Packet SendPacket;
    //ClientManager ClMan;
};

#endif
