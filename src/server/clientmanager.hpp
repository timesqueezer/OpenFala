#ifndef CLIENTMANAGER_HPP_INCLUDED
#define CLIENTMANAGER_HPP_INCLUDED

#include <SFML/Network.hpp>

#include <string>
#include <map>

struct Client {
    sf::IPAddress address;
    std::string name;
    sf::Uint16 port;

    sf::Uint16 posx; // Block behind client's cursor X
    sf::Uint16 posy; // and Y
};

class ClientManager {
    public:
        ClientManager(sf::Uint16 max_players=4);

        ~ClientManager();

        void Add(sf::IPAddress address, sf::Uint16 port, std::string name);

        void Remove(sf::Uint16 id);

        bool IsKnown(sf::IPAddress address);

        bool IsSlotAvailable();

        // Returns vector of currently active (connected) client IDs
        std::vector<sf::Uint16> GetIDs();

        sf::Uint16 GetID(sf::IPAddress address);

        // Get IP of client ID
        sf::IPAddress GetIP(sf::Uint16 id);

        sf::Uint16 GetPort(sf::Uint16 id);
    
        // Get name of client ID
        std::string GetName(sf::Uint16 id);

        // Get Block under cursor for client ID
        std::vector<sf::Uint16> GetBlockUnderCursor(sf::Uint16 id);
        
        sf::Uint16 GetMaxPlayers();

        sf::Uint16 GetActiveClients();

        // Set name of client ID
        void SetName(sf::Uint16 id, std::string name);

        // Set Block under cursor for client ID
        void SetBlockUnderCursor(sf::Uint16 id, sf::Uint16 posx, sf::Uint16 posy);

        void SetMaxPlayers(sf::Uint16 max_players);

    private:
        sf::Uint16 m_max_players;
        std::map<sf::Uint16, Client> m_clients;
};

#endif
