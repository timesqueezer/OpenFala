#ifndef NETWORK_HPP_INCLUDED
#define NETWORK_HPP_INCLUDED

#include <SFML/Network.hpp>

// packet_t is the header of each sent or received packet on either side,
// client or server. It determines which action to take on each side.
// If the packet is sent by the client, the server interprets it as an request.
// If the packet is sent by the server, the client interprets it as a command.
const sf::Uint16 PACKET_NONE = 0;
const sf::Uint16 PACKET_MOUSE = 1;
const sf::Uint16 PACKET_BUILD = 2;
const sf::Uint16 PACKET_HANDSHAKE = 3;
const sf::Uint16 PACKET_GOODBYE = 4;

#endif
