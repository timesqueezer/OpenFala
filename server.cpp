#include <SFML/System.hpp>

#include <iostream>

#include "server.hpp"

ServerApp::ServerApp(const uint16_t& port, const uint16_t& max_players) {
    Socket.Bind(port);
    m_port = port;
   	m_clist.resize(extents[2][max_players-1]);

	std::cout << "OpenFala server started on port: " << port << std::endl;
	std::cout << "Max players: " << max_players << std::endl;
}

ServerApp::~ServerApp() {}

void ServerApp::HandleRequest() {
 	sf::Uint16 mx = 0;
 	sf::Uint16 my = 0;

   	// IP Address reference only needed for connecting clients
   	Network::IPAddress claddress;
 	GetSocket().Receive(GetPacket(), claddress, m_port);
    GetPacket() >> mx >> my;
    std::cout << "MouseX: " << mx << " MouseY: " << my << std::endl;
    GetPacket().Clear();
}

void ServerApp::Update() {
}

Network::Socket& ServerApp::GetSocket() {
	return Socket;
}

Network::Packet& ServerApp::GetPacket() {
	return Packet;
}
