#ifndef CLIENT_HPP_INCLUDED
#define CLIENT_HPP_INCLUDED

#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/multi_array.hpp>

#include <ImageMagick/Magick++.h>

#include <string>

#include "network.hpp"
#include "filesystem.hpp"
#include "resources.hpp"
#include "block.hpp"

typedef boost::multi_array<Block*, 2> blockarray2d;

// Main class for client application.
class ClientApp {
public:
    ClientApp(const uint16_t&, const Network::IPAddress&,
    			const std::string&, const std::string&);
    ~ClientApp();

    // Initializes client.
    void Init();

    // Grabs user input and prepares it for sending to the server.
    void HandleInput();

	// Input grabbed by HandleInput() is sent to the server while getting new
	// info about the game state from it.
    void Update();

    // Draws all new data to the screens.
    void Draw();

   	// Safely shuts down the client.
    void Die();

    // Check if mouse is in legal area for building.
    bool MouseInPlayableArea();

    // Get the block beneath mouse cursor.
    sf::Uint16 GetMouseBlock(char);

    // Get the client socket.
    Network::Socket& GetSocket();
private:
	uint16_t m_width;
	uint16_t m_height;
	const sf::Input* input;
	uint16_t m_port;
	uint16_t m_clientport;
	std::string m_name;
    Network::IPAddress m_bindaddress;
    Network::Socket Socket;
    Network::Packet SendPacket;
    Network::Packet RecvPacket;

	sf::RenderWindow app;
	sf::Clock clock;
	ResourceManager ResMgr;
	Filesystem::path path;
	sf::String mousepos;
	float m_ratio;
	Block* highlightblock;

	blockarray2d::extent_gen extents;
	blockarray2d m_blocks;

	int blocknbx;
	int blocknby;

	int mode; // 0 - normal mode ; 1 - build mode ; 2 - demolition mode

	unsigned int highlightx, highlighty;

	#ifdef DEBUG
	sf::String fps;
	sf::Shape inforect;
	sf::Shape debugcircle;
	#endif
};

#endif
