#include <SFML/System.hpp>

#include <boost/foreach.hpp>

#include <iostream>
#include <string>

#include "../network/network.hpp"

#include "server.hpp"

// TODO Adding UID support for clients


// New Packet: PacketType(0, 1, 2), arg0(x, x, name), arg1(y, y, none)

ServerApp::ServerApp(const sf::Uint16 port, const sf::Uint16 max_players) {
    m_port = port;
    // TODO Blockmanager
    //m_blocks.resize(extents[20][10]); // Size of the playablearea which is everytime the same
    m_ClMan = ClientManager();
    m_ClMan.SetMaxPlayers(max_players);
}

ServerApp::~ServerApp() {}

void ServerApp::Init() {
    if (!Listener.Bind(m_port)) {
        std::cout << "Fail while binding the listening socket." << std::endl;
    }
    Selector.Add(Listener);
    std::cout << "OpenFala server started on port: " << m_port << std::endl;
    std::cout << "Max players: " << m_ClMan.GetMaxPlayers() << std::endl;

}

void ServerApp::HandleRequest() {

    unsigned int NbSockets = Selector.Wait(0.02f);

    if (NbSockets) {

        sf::SocketUDP cSocket = Selector.GetSocketReady(0); // Stands for currentSocket

        sf::Packet Packet;
        sf::IPAddress Address;
        sf::Uint16 port;
        cSocket.Receive(Packet, Address, port);
        sf::Uint16 command_id = 0;

        Packet >> command_id;

        if (command_id == PACKET_MOUSE) { // type "mouse", client sends mouse/block information
            sf::Uint16 sqx, sqy;

            Packet >> sqx >> sqy;
            
            std::cout << "Name " << m_ClMan.GetName(m_ClMan.GetID(Address)) << "[" << m_ClMan.GetID(Address) << "] : " << sqx << " " << sqy << std::endl;

            m_ClMan.SetBlockUnderCursor(m_ClMan.GetID(Address), sqx, sqy);

        } else if (command_id == PACKET_BUILD) { // "build" type for building blocks

            sf::Uint16 sqx, sqy;
            Packet >> sqx >> sqy;
            SendPacket << PACKET_BUILD << sqx << sqy << (sf::Uint16) m_ClMan.GetID(Address);
            cSocket.Send(SendPacket, Address, port);
            SendPacket.Clear();
            std::cout << "BUILD " << m_ClMan.GetName(m_ClMan.GetID(Address)) << "[" << m_ClMan.GetID(Address) << "] : " << sqx << " " << sqy << std::endl;

            BOOST_FOREACH(sf::Uint16 id, m_ClMan.GetIDs()) {
                if (id != m_ClMan.GetID(Address)) {
                    SendPacket << PACKET_BUILD << sqx << sqy << (sf::Uint16) m_ClMan.GetID(Address);
                    Listener.Send(SendPacket, m_ClMan.GetIP(id), port);
                    SendPacket.Clear();
                }
            }
        } else if (command_id == PACKET_HANDSHAKE) { // "Handshake" getting client's name
            if (!m_ClMan.IsKnown(Address)) {

                std::string cl_name;
                Packet >> cl_name;
                m_ClMan.Add(Address, port, cl_name);
                std::cout << "Added Client " << cl_name << "[" << Address << "]" << std::endl;
                SendPacket << PACKET_HANDSHAKE << (sf::Uint16) 1 << (sf::Uint16) 1 << (sf::Uint16) m_ClMan.GetID(Address);
                Listener.Send(SendPacket, Address, port);
                SendPacket.Clear();
            }

        } else {
            std::cout << "Bad command_id" << std::endl;
        }
        Packet.Clear();
    }
}


void ServerApp::Update() {
    BOOST_FOREACH(sf::Uint16 idtosend, m_ClMan.GetIDs()) {
        SendPacket << PACKET_MOUSE << m_ClMan.GetActiveClients();
        BOOST_FOREACH(sf::Uint16 iddata, m_ClMan.GetIDs()) {
            std::vector<sf::Uint16> tmpvec = m_ClMan.GetBlockUnderCursor(iddata);
            SendPacket << iddata << tmpvec[0] << tmpvec[1]; 
        }
        Listener.Send(SendPacket, m_ClMan.GetIP(idtosend), m_ClMan.GetPort(idtosend));
        SendPacket.Clear();
    }
}

void ServerApp::Die() {
    Listener.Close();
}
