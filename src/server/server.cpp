#include <SFML/System.hpp>

#include <boost/foreach.hpp>

#include <iostream>
#include <string>

#include "server.hpp"

// New Packet: PacketType(0, 1, 2), arg0(x, x, name), arg1(y, y, none)


ServerApp::ServerApp(const sf::Uint16& port, const sf::Uint16& maxplayers) {
    m_port = port; // port for incoming communication (default 41311)
    m_clientport = 41312; // port for outgoing communication
    m_maxplayers = maxplayers;
   	m_clist.resize(extents[4][maxplayers]);
    m_mpos.resize(extents[maxplayers][2]);
   	m_blocks.resize(extents[20][10]); // Size of the playablearea which is everytime the same
   	// Mouse positions m_mpos[player1,2,3,4][x,y]
   	m_active_clients = 0;


}

ServerApp::~ServerApp() {}

void ServerApp::Init() {
    if (!Listener.Bind(m_clientport)) {
        std::cout << "Fail while binding the listening socket." << std::endl;
    }
    Selector.Add(Listener);
	std::cout << "OpenFala server started on port: " << m_port << std::endl;
	std::cout << "Max players: " << m_maxplayers << std::endl;


}

void ServerApp::HandleRequest() {

    unsigned int NbSockets = Selector.Wait(0.2f);

    if (NbSockets) {

        sf::SocketUDP cSocket = Selector.GetSocketReady(0); // Stands for currentSocket

        sf::Packet Packet;
        sf::IPAddress Address;
        sf::Uint16 port;
        cSocket.Receive(Packet, Address, port);
        sf::Uint8 PacketType;

        Packet >> PacketType;

        if (PacketType == 0) { // type "mouse", client sends mouse/block information



        } else if (PacketType == 1) { // "build" type for building blocks

            sf::Uint16 sqx, sqy;
            Packet >> sqx >> sqy;

            SendPacket << (sf::Uint16) 1 << sqx << sqy << (sf::Uint8) 1;
            cSocket.Send(SendPacket, Address, port);
            SendPacket.Clear();

        } else if (PacketType == 2) { // "Handshake" getting client's name

            /*if (!ClMan.IsKnown(Address)) {

                std::string cl_name;
                Packet >> name;
                ClMan.Add(Address, name);

            }*/

        } else {

            std::cout << "Bad PacketType" << std::endl;

        }
        Packet.Clear();
    }
}


void ServerApp::Update() {
    for(int i = 0; i < m_maxplayers; ++i) {
        if (m_clist[0][i] != "") {
            for (int x = 0; x < m_maxplayers; ++x) {
                if (m_clist[0][x] != "") {
                    if (x != i) {
                        SendPacket << (sf::Uint16) 2 << (sf::Uint16) m_mpos[x][0] << (sf::Uint16) m_mpos[x][1] << (sf::Uint16) x;
                        Listener.Send(SendPacket, m_clist[0][i], 41312);
                        SendPacket.Clear();
                    }
                }
            }
        }
    }
}

void ServerApp::Die() {
    Listener.Close();
}

sf::SocketUDP& ServerApp::GetSocket() {
    return Listener;
}


sf::Packet& ServerApp::GetSendPacket() {
    return SendPacket;
}

int ServerApp::GetPlayerId(std::string name) {
    int playerid = 0;
    for(int i = 0; i < m_maxplayers; i++) {
        if(m_clist[1][i] == name) {
            playerid = i;
            break;
        }
    }
    return playerid;
}



/*


// First check if client is already in our list
bool isknown = false;
sf::Uint16 freeslot = 0;
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
break;
}
}

// If client is unknown, add client to the list
if(isknown == false) {
std::cout << "Adding new client: " << claddress << " \"" << name
<< "\"" << "freeslot: " << freeslot << std::endl;
m_clist[0][freeslot] = claddress.ToString();
m_clist[1][freeslot] = name;
++m_active_clients;
SendPacket << (sf::Uint16) 3 << (sf::Uint16) 0 << (sf::Uint16) 0 << (sf::Uint16) freeslot;
Socket.Send(SendPacket, m_clist[0][freeslot], 41312);
SendPacket.Clear();
}

std::cout << "Client " << name << ": " << cl_id << " SQX: " << sqx << " SQY: " << sqy << " type: " << buildtype << std::endl;

if (buildtype == "mouse") { // add mouse position to the list
m_mpos[cl_id][0] = sqx;
m_mpos[cl_id][1] = sqy;
for (int i; i < m_maxplayers; ++i) {
if (m_clist[0][i] != "") {
if (i != cl_id) {
SendPacket << (sf::Uint16) 2 << (sf::Uint16) m_mpos[cl_id][0] << (sf::Uint16) m_mpos[cl_id][1] << (sf::Uint16) i;
Socket.Send(SendPacket,m_clist[0][cl_id], 41312);
}
}
}

} else if (buildtype == "block") {
SendPacket << (sf::Uint16) 1 << sqx << sqy << cl_id;
Socket.Send(SendPacket, m_clist[0][cl_id], 41312);
SendPacket.Clear();
}
// Get ready for next package
Packet.Clear();


}






}*/


