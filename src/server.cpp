#include <SFML/System.hpp>

//#include <boost/foreach.hpp>

#include <iostream>
#include <string>

#include "server.hpp"

ServerApp::ServerApp(const uint16_t& port, const uint16_t& maxplayers) {
    m_port = port; // port for incoming communication (default 41311)
    m_clientport = 41312; // port for outgoing communication
    m_maxplayers = maxplayers;
   	m_clist.resize(extents[4][maxplayers]);
   	m_mpos.resize(extents[maxplayers][2]);
   	// Mouse positions m_mpos[player1,2,3,4][x,y]
}

ServerApp::~ServerApp() {}

void ServerApp::Init() {
    Socket.Bind(m_port);
	std::cout << "OpenFala server started on port: " << m_port << std::endl;
	std::cout << "Max players: " << m_maxplayers << std::endl;
}

void ServerApp::HandleRequest() {
   	// IP Address reference only needed for connecting clients
   	Network::IPAddress claddress;
 	Socket.Receive(RecvPacket, claddress, m_port);

    // We will expect client packages in this format
    // [0] name - client name
    // [1] sqx - horizontal square the client reports the mouse in
    // [2] sqy - vertical square the client reports the mouse in
    // [3] buildtype - what type if any of building the client wants to build
    std::string name;
    sf::Uint16 sqx;
    sf::Uint16 sqy;
    std::string buildtype;
    RecvPacket >> name >> sqx >> sqy >> buildtype;

    // Client list has format:
    // [0][0] client 0 IP
    // [1][0] client 0 name
    // TODO: [2][0] client 0 team
    // TODO: [3][0] client 0 money

    // First check whether client is already in our list
    bool isknown = false;
    uint16_t freeslot = 0;
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
    //std::cout << "Client: " << name << " SQX: " << sqx << " SQY: " << sqy << " type: " << buildtype << std::endl;
    #endif
    if (buildtype == "mouse") {
        m_mpos[GetPlayerId(name)][0] = sqx;
        m_mpos[GetPlayerId(name)][1] = sqy;
        std::cout << "Success adding xy to mpos" << sqx << " " << sqy << std::endl;
    }
    // Get ready for next package
    RecvPacket.Clear();
}

void ServerApp::Update() {
    for(int i = 0; i < m_maxplayers; i++) {
        if (m_clist[0][i] != "") {
            SendPacket >> (sf::Uint16) i >> m_mpos[i][0] >> m_mpos[i][1];
            Socket.Send(SendPacket, m_clist[0][i], 41312);
            SendPacket.Clear();
        }
    }
}

void ServerApp::Die() {
    Socket.Close();
}

Network::Socket& ServerApp::GetSocket() {
	return Socket;
}

Network::Packet& ServerApp::GetRecvPacket() {
	return RecvPacket;
}

Network::Packet& ServerApp::GetSendPacket() {
    return SendPacket;
}

sf::Uint8 ServerApp::GetPlayerId(std::string name) {
    for(int i = 0; i < m_maxplayers; i++) {
        if(m_clist[1][i] == name) {
            return (sf::Uint8) i;
        }
    }
    return false;
}
