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
#include "utility.hpp"
#include "block.hpp"

typedef boost::multi_array<Block*, 2> blockarray2d;

class ClientApp {
public:
    ClientApp(const uint16_t&, const Network::IPAddress&, const std::string&);
    ~ClientApp();

    void Update();
    void Draw();

private:
	uint16_t width;
	uint16_t height;
	const sf::Input* input;
	uint16_t port;
    Network::IPAddress bind_address;
    Network::Socket Socket;
    Network::Packet Packet;
	sf::RenderWindow app;
	sf::Clock clock;
	utility::ResourceLoader Loader;
	filesystem::path path;
	sf::String mousepos;

	blockarray2d::extent_gen extents;
	blockarray2d m_blocks;

	int blocknbx;
	int blocknby;
	
	#ifdef DEBUG
	sf::String fps;
	sf::Shape inforect;
	sf::Shape debugcircle;
	#endif
};

#endif
