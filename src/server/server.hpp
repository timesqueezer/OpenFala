#ifndef SERVER_HPP_INCLUDED
#define SERVER_HPP_INCLUDED

#include <boost/multi_array.hpp>

#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include <string>

#include "clientmanager.hpp"
#include "../world/World.hpp"

typedef boost::multi_array<std::string, 2> stringarray2d;
typedef boost::multi_array<sf::Uint16, 2> uint16array2d;

// Main class for server application.
class ServerApp {
public:
    ServerApp(const sf::Uint16 port, const sf::Uint16 max_players);
    ~ServerApp();

    // Initializes server.
    void Init();

    // Listens for incoming client requests and acts accordingly.
    // This method takes care of client registering and data gathering.
	void HandleRequest();

	// Updates all Entities, calculates physics
	void Update();

	// Sends snapshots to clients
	void Synchronize();

	// Safely shuts down the server.
	void Die();

private:
    World mWorld;

    sf::Uint16 m_port;
    uint16array2d m_blocks;
    sf::SelectorUDP Selector;
    sf::SocketUDP Listener;
    sf::Packet SendPacket;
    ClientManager m_ClMan;
};

#endif
