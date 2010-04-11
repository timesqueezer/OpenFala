#include <iostream>
#include <string>
#include <SFML/Network.hpp>

int main(void) {

    enum PacketType {
        Handshake,
        Msg,
        Default
    };
    
    PacketType t_msg = Msg;
    PacketType t_hs = Handshake;

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
    //Packet << t_hs << "Hallo";
//    Socket.Send(Packet, Address, port);
  //  Packet.Clear();

    while (true) {
//        Packet << t_hs << t_msg;
   //     std::cout << "Data to send to the server: ";
//        std::cin >> data;
        data = "lol";

        Packet << t_msg;

        Socket.Send(Packet, Address, port);

        Packet.Clear();
    }

}
