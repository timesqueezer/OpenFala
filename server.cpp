#include <SFML/System.hpp>

#include <iostream>
#include <string>

#include "server.hpp"

ServerApp::ServerApp(const uint16_t& port, const uint16_t& maxplayers) {
    Socket.Bind(port);
    m_port = port;
    m_maxplayers = maxplayers;
   	m_clist.resize(extents[4][maxplayers]);

	std::cout << "OpenFala server started on port: " << port << std::endl;
	std::cout << "Max players: " << maxplayers << std::endl;
}

ServerApp::~ServerApp() {}

void ServerApp::HandleRequest() {
   	// IP Address reference only needed for connecting clients
   	Network::IPAddress claddress;
 	GetSocket().Receive(GetPacket(), claddress, m_port);

    // We will expect client packages in this format
    // [0] name - client name
    // [1] sqx - horizontal square the client reports the mouse in
    // [2] sqy - vertical square the client reports the mouse in
    // [3] buildtype - what type if any of building the client wants to build
    std::string name;
    sf::Uint16 sqx;
    sf::Uint16 sqy;
    std::string buildtype;
    GetPacket() >> name >> sqx >> sqy >> buildtype;

    // Client list has format:
    // [0][0] client 0 IP
    // [1][0] client 0 name
    // TODO: [2][0] client 0 team
    // TODO: [3][0] client 0 money

    // First check whether client is already in our list
    bool isknown = false;
    uint16_t freeslot;
    std::string ip = claddress.ToString();
    for(int i = 0; i < m_maxplayers; i++) {
        // Only check if slot is filled by client
        if(m_clist[0][i] != "") {
            if(ip == m_clist[0][i]) {
                isknown = true;
                break;
            }
        }
        else {
            // Record free slot for later
            freeslot = i;
        }
    }

    // If client is unknown, add client to the list
    if(isknown == false) {
        std::cout << "Adding new client: " << claddress << " \"" << name
            << "\"" << std::endl;
        m_clist[0][freeslot] = claddress.ToString();
        m_clist[1][freeslot] = name;
    }

    #ifdef DEBUG
    std::cout << "MouseX: " << sqx << " MouseY: " << sqy << std::endl;
    #endif

    // Get ready for next package
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
