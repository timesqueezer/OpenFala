#include <iostream>
#include <string>
#include <SFML/Network.hpp>
#include "lol.cpp"

int main() {
    sf::SocketUDP Listener;
    sf::Packet Packet;

    sf::IPAddress Address;
    sf::Uint16 port;

    if (!Listener.Bind(12345)) {
        std::cout << "Fail while binding the listening Socket"; 
    }

    Listener.Receive(Packet, Address, port);

    Derived d("falsch",0);
    Packet >> d;
    d.printInfo();

    Listener.Close();

    return EXIT_SUCCESS;
}
