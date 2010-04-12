#include <iostream>
#include <SFML/Network.hpp>

int main() {

    sf::SelectorUDP Selector;

    sf::SocketUDP Listener;

    if (!Listener.Bind(12345)) {
        std::cout << "Fail while binding the listening Socket"; 
    }

    Selector.Add(Listener);



    while (true) {
    
        unsigned int NbSockets = Selector.Wait(0.2f);

        if (NbSockets) {
            
            sf::SocketUDP cSocket = Selector.GetSocketReady(0); // Stands for currentSocket

            sf::Packet Packet;
            sf::IPAddress Address;
            unsigned short port;
            cSocket.Receive(Packet, Address, port);
            std::string data;

            Packet >> data;
            std::cout << "Client [" << Address << ":" << port << "] sent: " << data << std::endl;
            
            Packet.Clear();

        }
    
    }
    Listener.Close();
    return EXIT_SUCCESS;
}
