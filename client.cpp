#include <SFML/System.hpp>
#include <iostream>

#include <boost/filesystem.hpp>

#include "client.hpp"
#include "filesystem.hpp"

Client::Client(uint16_t port, network::IPAddress ipaddress) {
}

Client::~Client() {}

void Client::create_window() {
	App.Create(sf::VideoMode::GetMode(0), "OpenFala", 0,
	sf::WindowSettings::WindowSettings ( 24, 8, 4));
}
