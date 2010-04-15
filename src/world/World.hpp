#ifndef _WORLD_HPP_INCLUDED_
#define _WORLD_HPP_INCLUDED_

#include <boost/ptr_container/ptr_container.hpp>
#include <SFML/Network.hpp>

#include "../entity/IEntity.hpp"

// Type of Entity Map
typedef class boost::ptr_map<sf::Uint16, IEntity> EntityMap;

class World{
public:
    // Default constructor
    World() {}

    // Default destructor
    ~World() {}

    // Adds an entity to the internal entity list mEntities
    void AddEntity(IEntity* entity);

    // Return internal entity list mEntities
    EntityMap& GetEntities();

    // Calls all Entity Update methods, calculates physics
    void Update();

    // Draws all Entities to RenderTarget
    void Draw(sf::RenderTarget& target, float blocksize, sf::Vector2f offset);

    // To send snapshot via sf::Socket / friends are needed to give the
    // operator functions access to private Members of the World object
    friend sf::Packet &operator<<(sf::Packet &packet, World &w);
    friend sf::Packet &operator>>(sf::Packet &packet, World &w);
private:
    EntityMap mEntities;
    sf::Clock frameClock;
    sf::Uint16 m_entity_index;
};

#endif
