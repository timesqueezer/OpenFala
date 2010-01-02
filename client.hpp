#ifndef CLIENT_HPP_INCLUDED
#define CLIENT_HPP_INCLUDED

#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <ImageMagick/Magick++.h>

#include <string>

#include "network.hpp"
#include "filesystem.hpp"

class Client {
public:
    Client(uint16_t, network::IPAddress);
    ~Client();
    uint16_t port;
    network::Socket Socket;
    network::Packet Packet;
	sf::RenderWindow App;
	sf::Clock Clock;
	sf::Shape Circle;
	sf::String FPS;
	void create_window();
private:
};

#endif
