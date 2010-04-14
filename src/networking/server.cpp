#include <iostream>
#include <string>
#include <SFML/Network.hpp>

enum PacketType {
    LOL,
    OMFG,
    NIX
};

int main() {
    sf::SocketUDP Listener;
    sf::Packet Packet;

    sf::IPAddress Address;
    sf::Uint16 port;

    if (!Listener.Bind(12345)) {
        std::cout << "Fail while binding the listening Socket"; 
    }

    Listener.Receive(Packet, Address, port);

    sf::Uint16 bar = 41;
    Packet >> bar;
    std::cout << "bar: " << bar;

    Listener.Close();

    return EXIT_SUCCESS;
}
