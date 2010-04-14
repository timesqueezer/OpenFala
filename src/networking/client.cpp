#include <iostream>
#include <string>
#include <SFML/Network.hpp>

enum PacketType {
    LOL,
    OMFG,
    NIX
};

int main(void) {
    sf::SocketUDP Socket;
    sf::Packet Packet;

    sf::IPAddress Address("localhost");
    sf::Uint16 port = 12345;

    PacketType foo = NIX;
    sf::Uint16 foo2 = static_cast<sf::Uint16>(foo);
    Packet << foo2; 
    std::cout << "LOL: " << foo;

    Socket.Send(Packet, Address, port);

    return EXIT_SUCCESS;
}
