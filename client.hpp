#ifndef CLIENT_HPP_INCLUDED
#define CLIENT_HPP_INCLUDED

#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <ImageMagick/Magick++.h>

#include <string>

#include "network.hpp"

#include "filesystem.hpp"
#include "utility.hpp"

class Client {
public:
    Client(const uint16_t&, const network::IPAddress&, const std::string&);
    ~Client();
    uint16_t port;
    network::IPAddress bind_address;
    network::Socket socket;
    network::Packet packet;
	sf::RenderWindow app;
	sf::Clock clock;
	sf::String fps;
	const sf::Input* input;
private:
	uint16_t width;
	uint16_t height;
};

#endif
