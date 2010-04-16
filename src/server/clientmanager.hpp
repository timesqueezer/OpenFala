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
        ClientManager(const sf::Uint16 max_players=4);

        ~ClientManager();

        void Add(const sf::IPAddress address, const sf::Uint16 port, const std::string name);

        void Remove(const sf::Uint16 id);

        bool IsKnown(const sf::IPAddress address);

        bool IsSlotAvailable();

        // Returns vector of currently active (connected) client IDs
        std::vector<sf::Uint16> GetIDs();

        sf::Uint16 GetID(const sf::IPAddress address);

        // Get IP of client ID
        sf::IPAddress GetIP(const sf::Uint16 id);

        sf::Uint16 GetPort(const sf::Uint16 id);
    
        // Get name of client ID
        std::string GetName(const sf::Uint16 id);

        // Get Block under cursor for client ID
        std::vector<sf::Uint16> GetBlockUnderCursor(const sf::Uint16 id);
        
        sf::Uint16 GetMaxPlayers();

        sf::Uint16 GetActiveClients();

        // Set name of client ID
        void SetName(const sf::Uint16 id, const std::string name);

        // Set Block under cursor for client ID
        void SetBlockUnderCursor(const sf::Uint16 id, const sf::Uint16 posx, const sf::Uint16 posy);

        void SetMaxPlayers(const sf::Uint16 max_players);

    private:
        sf::Uint16 m_max_players;
        std::map<sf::Uint16, Client> m_clients;
};

#endif
