#include <SFML/System.hpp>

#include <boost/foreach.hpp>

#include <iostream>
#include <string>

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
            
            sf::Uint16 sqx, sqy;

            Packet >> sqx >> sqy;
            
            std::cout << "Name " << m_ClMan.GetName(m_ClMan.GetID(Address)) << "[" << m_ClMan.GetID(Address) << "] : " << sqx << " " << sqy << std::endl;

            m_ClMan.SetBlockUnderCursor(m_ClMan.GetID(Address), sqx, sqy);

            BOOST_FOREACH(int id, m_ClMan.GetIDs()) {
                if (id != m_ClMan.GetID(Address)) {
                    SendPacket << (sf::Uint16) 2 << sqx << sqy << (sf::Uint16) m_ClMan.GetID(Address);
                    Listener.Send(SendPacket, m_ClMan.GetIP(id), port);
                    SendPacket.Clear();
                }
            }
            
        
        } else if (PacketType == 1) { // "build" type for building blocks

            sf::Uint16 sqx, sqy;
            Packet >> sqx >> sqy;
            SendPacket << (sf::Uint16) 1 << sqx << sqy << (sf::Uint8) m_ClMan.GetID(Address);
            cSocket.Send(SendPacket, Address, port);
            SendPacket.Clear();
            std::cout << "BUILD " << m_ClMan.GetName(m_ClMan.GetID(Address)) << "[" << m_ClMan.GetID(Address) << "] : " << sqx << " " << sqy << std::endl;

            BOOST_FOREACH(int id, m_ClMan.GetIDs()) {
                if (id != m_ClMan.GetID(Address)) {
                    SendPacket << (sf::Uint16) 1 << sqx << sqy << (sf::Uint16) m_ClMan.GetID(Address);
                    Listener.Send(SendPacket, m_ClMan.GetIP(id), port);
                    SendPacket.Clear();
                }
            }

        } else if (PacketType == 2) { // "Handshake" getting client's name


            if (!m_ClMan.IsKnown(Address)) {

                std::string cl_name;
                Packet >> cl_name;
                m_ClMan.Add(Address, cl_name);
                std::cout << "Added Client " << cl_name << "[" << Address << "]" << std::endl;
                SendPacket << (sf::Uint8) 3 << (sf::Uint16) 1 << (sf::Uint16) 1 << (sf::Uint16) m_ClMan.GetID(Address);

            }


        } else {

            std::cout << "Bad PacketType" << std::endl;

        }
        Packet.Clear();
    }
}


void ServerApp::Update() {}

void ServerApp::Die() {
    Listener.Close();
}
