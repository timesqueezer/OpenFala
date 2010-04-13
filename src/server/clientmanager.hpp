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

    // Get 
    std::vector<sf::Uint8> GetBlockUnderCursor(sf::Uint8 id);
private:

