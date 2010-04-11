struct Client {
    sf::IPAddress address;
    std::string name;

    sf::Uint8 posx; // Block behind client's cursor X
    sf::Uint8 posy; // and Y
};

class ClientManager {
    public:
        ClientManager(sf::Uint8 max_players);

        ~ClientManager();

        void Add(sf::IPAddress address, std::string name)

        void Remove(sf::IPAddress address);

        bool IsKnown(sf::IPAddress address);

        bool IsSlotAvailable();

        // Returns vector of currently active (connected) client IDs
        std::vector<sf::Uint8> GetIDs();

        // Get IP of client ID
        sf::IPAddress GetIP(sf::Uint8 id);
    
        // Get name of client ID
        std::string GetName(sf::Uint8 id);

        // Get Block under cursor for client ID
        std::vector<sf::Uint8> GetBlockUnderCursor(sf::Uint8 id);
        
        // Set name of client ID
        void SetName(sf::Uint8 id, std::string name);

        // Set Block under cursor for client ID
        void SetBlockUnderCursor(sf::Uint8 id, sf::Uint8 posx, sf::Uint8 posy);

    private:
        sf::Uint8 m_max_players;

        std::map<sf::Uint8, Client> m_clients;
};
