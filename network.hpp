#ifndef NETWORK_HPP_INCLUDED
#define NETWORK_HPP_INCLUDED

#include <SFML/Network.hpp>

namespace network {
    typedef sf::SocketUDP Socket;
    typedef sf::SelectorUDP Selector;
    typedef sf::Packet Packet;
    typedef sf::IPAddress IPAddress;
}

#endif
