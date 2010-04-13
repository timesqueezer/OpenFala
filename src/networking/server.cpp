#include <iostream>
#include <string>
#include <SFML/Network.hpp>

int main() {

    enum PacketType {
        Handshake,
        Msg,
        Default
    };

    sf::SelectorUDP Selector;

    sf::SocketUDP Listener;

    if (!Listener.Bind(12345)) {
        std::cout << "Fail while binding the listening Socket"; 
    }

    Selector.Add(Listener);

    //Listener.SetBlocking(true);

    while (true) {
    
        unsigned int NbSockets = Selector.Wait(0.2f);

        if (NbSockets) {
            
            sf::SocketUDP cSocket = Selector.GetSocketReady(0); // Stands for currentSocket

            sf::Packet Packet;
            sf::IPAddress Address;
            unsigned short port;
            cSocket.Receive(Packet, Address, port);
            std::string data;
            PacketType t_hs = Default;
            PacketType t_msg = Default;
            int i = 0;
            Packet >> i;
            std::cout << i;
            //std::cout << "Client [" << Address << ":" << port << "] sent: " << data << " with type: " << t_hs << " " << t_msg << std::endl;
            
            Packet.Clear();
            //sf::Sleep(0.2f);

        }
    
    }
    Listener.Close();
    return EXIT_SUCCESS;
}
