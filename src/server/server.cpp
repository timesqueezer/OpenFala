#include <SFML/System.hpp>

#include <boost/foreach.hpp>

#include <iostream>
#include <string>

#include "../network/network.hpp"
#include "../entity/Building.hpp"

#include "server.hpp"

const sf::Uint16 MODE_BUILD = 1;
const sf::Uint16 MODE_DEMOLISH = 2;


// New Packet: PacketType(0, 1, 2), arg0(x, x, name), arg1(y, y, none)

ServerApp::ServerApp(const sf::Uint16 port, const sf::Uint16 max_players) {
    m_port = port;
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
            m_ClMan.SetBlockUnderCursor(m_ClMan.GetID(Address), sqx, sqy);

        } else if (command_id == PACKET_BUILD) { // "build" type for building something
            sf::Uint16 mode = 0;
            Packet >> mode; // get mode (BUILD/DEMOLISH)
            sf::Uint16 sqx, sqy;
            Packet >> sqx >> sqy;
            if (mode == MODE_BUILD) {
                // if the block is not in the earth AND there is no block at that position AND
                // (the position is on the ground OR there is a block underneath)
                if (sqy < 10 and !mWorld.BlockExistsAt(sqx,sqy) and (sqy==9 or mWorld.BlockExistsAt(sqx,sqy+1))) {
                    // Create entity
                    Building* b = new Building();
                    b->SetPosition(sf::Vector2f(sqx, sqy));
                    b->SetDimension(sf::Vector2f(1,1));
                    mWorld.AddEntity(b);
                }
            } else if (mode == MODE_DEMOLISH) {
                // if the block exists and there is no block above
                if (mWorld.BlockExistsAt(sqx,sqy) and !mWorld.BlockExistsAt(sqx,sqy-1)) {
                    mWorld.DeleteEntity(sqx, sqy, ENTITY_TYPE_BUILDING);
                }
            }

        } else if (command_id == PACKET_HANDSHAKE) { // "Handshake" getting client's name

            if (!m_ClMan.IsKnown(Address)) {
                std::string cl_name;
                Packet >> cl_name;
                m_ClMan.Add(Address, port, cl_name);
                std::cout << "Added Client " << cl_name << "[" << Address << "][id" << m_ClMan.GetID(Address) << "]" << std::endl;
                SendPacket << PACKET_HANDSHAKE << (sf::Uint16) m_ClMan.GetID(Address);
                Listener.Send(SendPacket, Address, port);
                SendPacket.Clear();

                // SEND THE FIRST SNAPSHOT
                SendPacket << PACKET_SNAPSHOT << mWorld;
                Listener.Send(SendPacket, Address, port);
                SendPacket.Clear();
            }

        } else if (command_id == PACKET_GOODBYE) {

            sf::Uint16 cl_id;
            Packet >> cl_id;
            m_ClMan.Remove(cl_id);
            std::cout << "Removed client[" << cl_id << "][" << Address << "]" << std::endl;

        } else {
            std::cout << "Bad command_id" << std::endl;
        }
        Packet.Clear();
    }
}

void ServerApp::Update() {
    mWorld.Update(0.0); // no need for blocksize for server
}

void ServerApp::Synchronize() {
    BOOST_FOREACH(sf::Uint16 idtosend, m_ClMan.GetIDs()) {
        SendPacket << PACKET_SNAPSHOT << mWorld;
        Listener.Send(SendPacket, m_ClMan.GetIP(idtosend), m_ClMan.GetPort(idtosend));
        SendPacket.Clear();
    }

}

void ServerApp::Die() {
    Listener.Close();
}
