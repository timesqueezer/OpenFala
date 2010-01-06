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

class ClientApp {
public:
    ClientApp(const uint16_t&, const Network::IPAddress&,
    			const std::string&, const std::string&);
    ~ClientApp();

    void Init();
    void HandleInput();
    void Update();
    void Draw();
    bool MouseInPlayableArea();
    sf::Uint16 GetMouseBlock(char);
    Network::Socket& GetSocket();

private:
	uint16_t width;
	uint16_t height;
	const sf::Input* input;
	uint16_t port;
	std::string m_name;
    Network::IPAddress bind_address;
    Network::Socket Socket;
    Network::Packet Packet;
	sf::RenderWindow app;
	sf::Clock clock;
	ResourceManager ResMgr;
	Filesystem::path path;
	sf::String mousepos;
	float ratio;
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
