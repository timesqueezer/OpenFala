#include <iostream>
#include <SFML/Network.hpp>

int main(void) {
    
    sf::SocketUDP Socket;
    sf::Packet Packet;

    sf::IPAddress Address;

    unsigned short port = 12345;

    std::string data = "";
    std::cout << "IPAddress to connect to: ";
    std::cin >> Address;

    if (!Address.IsValid()) {
        std::cout << "fail" << std::endl;
        exit(0);
    }

    while (true) {
        std::cout << "Data to send to the server: ";
        std::cin >> data;

        Packet << data;

        Socket.Send(Packet, Address, port);

        Packet.Clear();
    }

}
