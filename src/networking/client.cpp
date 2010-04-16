#include <iostream>
#include <string>
#include <SFML/Network.hpp>
#include "lol.cpp"

int main(void) {
    sf::SocketUDP Socket;
    sf::Packet Packet;

    sf::IPAddress Address("localhost");
    sf::Uint16 port = 12345;

    Derived d("myname",22);

    Packet << d; 

    Socket.Send(Packet, Address, port);

    return EXIT_SUCCESS;
}
