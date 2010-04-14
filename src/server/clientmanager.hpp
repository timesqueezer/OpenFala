#ifndef CLIENTMANAGER_HPP_INCLUDED
#define CLIENTMANAGER_HPP_INCLUDED

#include <SFML/Network.hpp>

#include <string>
#include <map>

struct Client {
    sf::IPAddress address;
    std::string name;

    sf::Uint8 posx; // Block behind client's cursor X
    sf::Uint8 posy; // and Y
};

class ClientManager {
    public:
        ClientManager(sf::Uint8 max_players=4);

        ~ClientManager();

        void Add(sf::IPAddress address, std::string name);

        void Remove(sf::Uint8 id);

        bool IsKnown(sf::IPAddress address);

        bool IsSlotAvailable();

        // Returns vector of currently active (connected) client IDs
        std::vector<int> GetIDs();

        int GetID(sf::IPAddress address);

        // Get IP of client ID
        sf::IPAddress GetIP(sf::Uint8 id);
    
        // Get name of client ID
        std::string GetName(sf::Uint8 id);

        // Get Block under cursor for client ID
        std::vector<sf::Uint8> GetBlockUnderCursor(sf::Uint8 id);
        
        sf::Uint16 GetMaxPlayers();

        // Set name of client ID
        void SetName(sf::Uint8 id, std::string name);

        // Set Block under cursor for client ID
        void SetBlockUnderCursor(sf::Uint8 id, sf::Uint8 posx, sf::Uint8 posy);

        void SetMaxPlayers(sf::Uint16 max_players);

    private:
        sf::Uint8 m_max_players;

        std::map<int, Client> m_clients;
};

#endif
