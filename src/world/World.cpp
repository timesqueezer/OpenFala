#include <boost/foreach.hpp>
#include <iostream>
#include "World.hpp"
#include "../entity/Building.hpp"

void World::AddEntity(IEntity* entity) {
    mEntities.insert(++m_entity_index, entity);
}

EntityMap& World::GetEntities() {
    return mEntities;
}


// Calls all Entity Update methods, calculates physics
void World::Update(float blocksize){

    // reset frame Clock before frame Calculations, but remember time
    float frameTime = frameClock.GetElapsedTime();
    frameClock.Reset();

    // Iterate through all Entities and Update them
    for ( EntityMap::iterator i = mEntities.begin(); i!=mEntities.end(); ++i ){
        i->second->Update(blocksize, frameTime);
    }

}

// Draws all Entities to RenderTarget
void World::Draw(sf::RenderTarget& target, float blocksize, sf::Vector2f offset){
    // Iterate through all Entities and Draw their Sprites
    for ( EntityMap::iterator i = mEntities.begin(); i!=mEntities.end(); ++i ){
        sf::Sprite& s = i->second->GetSprite(blocksize, offset);
        target.Draw(s);
    }
}


bool World::BlockExistsAt(float x, float y) const {
    for ( EntityMap::const_iterator i = mEntities.begin(); i!=mEntities.end(); ++i ){
        if (i->second->GetPosition().x == x and i->second->GetPosition().y == y and i->second->GetImageKey()=="building")
            return true;
    }
    return false;
}


// To Send a snapshot via Socket Connection
sf::Packet &operator<<(sf::Packet& packet, World& world){

    // Only the Server needs m_entity_index, as he gives new objects
    // their names...

    // frameClock is individually for every client

    /* Packet structure:

        - Number of Entities
        - Key of Entity 1
        - Type of Entity 1
        - Entity 1
        - Key of Entity 2
        - Type of Entity 2
        - Entity 2
        ...

    */

    // Send number of entites
    packet << (sf::Uint16)(world.mEntities.size());

    // Iterate through all Entities and send their keys, types and them
    for (EntityMap::iterator i = world.mEntities.begin(); i!=world.mEntities.end(); ++i){
        // Send Entity key
        packet << (sf::Uint16)i->first;

        // Send Entity Type
        sf::Uint16 entity_type = i->second->GetType();
        packet << entity_type;

        // stream entity
        //Building* entity;
        if (entity_type == ENTITY_TYPE_BUILDING){
            Building* entity =  dynamic_cast<Building*>(i->second);
            packet << *entity;
        }
        else
            std::cerr << "ERROR: Invalid Entity type." << std::endl;
/*
        entity = (Building*)i->second;

        packet << *entity;*/
    }

    return packet;
}

// To extract a snapshot from Socket Connection
sf::Packet &operator>>(sf::Packet &packet, World &world)
{
    // as every entity would be overwritten from the stream, remove all
    // existing entities
    world.mEntities.clear();

    // Get Number of Entities
    sf::Uint16 nbEntities;
    packet >> nbEntities;

    for (sf::Uint16 i = 0; i<nbEntities; ++i){
        sf::Uint16 key;
        packet >> key;

        // Get Entity Type
        sf::Uint16 entity_type;
        packet >> entity_type;

        /*IEntity* entity;
        if (entity_type == ENTITY_TYPE_BUILDING)
            entity = new Building();
        else
            std::cerr << "ERROR: Invalid Entity type." << std::endl;

        // get entity
        packet >> (*entity);
*/

        if (entity_type == ENTITY_TYPE_BUILDING){
            Building* entity = new Building(); // =  dynamic_cast<Building*>(i->second);
            packet >> *entity;
            world.mEntities.insert(key, entity);
            //std::cout << "lol gugg mal: 0 = " << entity.GetType() << std::endl;
            //entity.Update(0.0);
        }
        else
            std::cerr << "ERROR: Invalid Entity type." << std::endl;

    }

    return packet;
}
