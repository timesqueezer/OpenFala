#include <SFML/Network.hpp>

#include <iostream>
#include <string>
#include <map>

#include "clientmanager.hpp"

ClientManager::ClientManager(sf::Uint16 max_players) {
    m_max_players = max_players;
}

ClientManager::~ClientManager() {}

void ClientManager::Add(sf::IPAddress address, sf::Uint16 port, std::string name) {
    
    std::map<sf::Uint16, Client>::iterator it;
    it = m_clients.end();
    Client client;
    client.address = address;
    client.port = port;
    client.name = name;
    client.posx = 0;
    client.posy = 0;
    std::cout << it->first << std::endl << m_clients.size() << std::endl;
    m_clients.insert(std::pair<sf::Uint16, Client>(it->first, client));
}

void ClientManager::Remove(sf::Uint16 id) {
    m_clients.erase(id);
}

bool ClientManager::IsKnown(sf::IPAddress address) {
    bool isknown = false;
    std::map<sf::Uint16, Client>::iterator it;
    for(it=m_clients.begin(); it != m_clients.end(); ++it) {
        if (it->second.address == address) {
            isknown = true;
            break;
        }
    }
    return isknown;
}

bool ClientManager::IsSlotAvailable() {
    if (m_clients.size() >= m_max_players) {
        return false;
    } else {
        return true;
    }
}

std::vector<sf::Uint16> ClientManager::GetIDs() {
    std::vector<sf::Uint16> ids;
    std::map<sf::Uint16, Client>::iterator it;
    sf::Uint16 tmp = 0;
    for ( it=m_clients.begin() ; it != m_clients.end(); it++ ) {
        tmp = (*it).first;
        ids.push_back(tmp);
    }
    return ids;
}

sf::Uint16 ClientManager::GetID(sf::IPAddress address) {
    std::map<sf::Uint16, Client>::iterator it;
    sf::Uint16 tmp = 0;
    for ( it = m_clients.begin() ; it != m_clients.end(); it++ ) {
        if (it->second.address == address) {
            tmp = it->first;
            break;
        }
    }
    return tmp;

}

sf::IPAddress ClientManager::GetIP(sf::Uint16 id) {
   return m_clients[id].address;
}

sf::Uint16 ClientManager::GetPort(sf::Uint16 id) {
    return m_clients[id].port;
}

std::string ClientManager::GetName(sf::Uint16 id) {
    return m_clients[id].name;
}

std::vector<sf::Uint16> ClientManager::GetBlockUnderCursor(sf::Uint16 id) {
    std::vector<sf::Uint16> blocks;
    blocks.push_back(m_clients[id].posx);
    blocks.push_back(m_clients[id].posy);
    return blocks;
}

sf::Uint16 ClientManager::GetMaxPlayers() {
    return m_max_players;
}

sf::Uint16 ClientManager::GetActiveClients() {
    return m_clients.size();
}

void ClientManager::SetName(sf::Uint16 id, std::string name) {
    m_clients[id].name = name;
}

void ClientManager::SetBlockUnderCursor(sf::Uint16 id, sf::Uint16 posx, sf::Uint16 posy) {
    m_clients[id].posx = posx; 
    m_clients[id].posy = posy; 
}

void ClientManager::SetMaxPlayers(sf::Uint16 max_players) {
    m_max_players = max_players;
}
