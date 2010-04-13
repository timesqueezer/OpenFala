#include <SFML/Network.hpp>

#include <iostream>
#include <string>
#include <map>

#include "clientmanager.hpp"

ClientManager::ClientManager(sf::Uint8 max_players) {
    m_max_players = max_players;
}

ClientManager::~ClientManager() {}

void ClientManager::Add(sf::IPAddress address, std::string name) {
    
    std::map<sf::Uint8, Client>::iterator it;
    it = m_clients.end();
    Client client;
    client.address = address;
    client.name = name;
    client.posx = 0;
    client.posy = 0;
    m_clients.insert(std::pair<sf::Uint8, Client>(it->first, client));
}

void ClientManager::Remove(sf::Uint8 id) {
    m_clients.erase(id);
}

bool ClientManager::IsKnown(sf::IPAddress address) {
    bool isknown = false;
    std::map<sf::Uint8, Client>::iterator it;
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

std::vector<sf::Uint8> ClientManager::GetIDs() {
    std::vector<sf::Uint8> ids;
    std::map<sf::Uint8, Client>::iterator it;
    sf::Uint8 tmp = 0;
    for ( it=m_clients.begin() ; it != m_clients.end(); it++ ) {
        tmp = (*it).first;
        ids.push_back(tmp);
    }
    return ids;
}

sf::IPAddress ClientManager::GetIP(sf::Uint8 id) {
   return m_clients[id].address;
}

std::string ClientManager::GetName(sf::Uint8 id) {
    return m_clients[id].name;
}

std::vector<sf::Uint8> ClientManager::GetBlockUnderCursor(sf::Uint8 id) {
    std::vector<sf::Uint8> blocks;
    blocks.push_back(m_clients[id].posx);
    blocks.push_back(m_clients[id].posy);
    return blocks;
}

sf::Uint16 ClientManager::GetMaxPlayers() {
    return m_max_players;
}

void ClientManager::SetName(sf::Uint8 id, std::string name) {
    m_clients[id].name = name;
}

void ClientManager::SetBlockUnderCursor(sf::Uint8 id, sf::Uint8 posx, sf::Uint8 posy) {
    m_clients[id].posx = posx; 
    m_clients[id].posy = posy; 
}

void ClientManager::SetMaxPlayers(sf::Uint16 max_players) {
    m_max_players = max_players;
}
