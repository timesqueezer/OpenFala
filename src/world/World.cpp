#include "World.hpp"
#include <boost/foreach.hpp>
#include <iostream>

void World::AddEntity(IEntity* entity) {
    mEntities.insert(++m_entity_index, entity);
}

EntityMap& World::GetEntities() {
    return mEntities;
}


// Calls all Entity Update methods, calculates physics
void World::Update(){

    // reset frame Clock before frame Calculations, but remember time
    float frameTime = frameClock.GetElapsedTime();
    frameClock.Reset();

    // Iterate through all Entities and Update them
    for ( EntityMap::iterator i = mEntities.begin(); i!=mEntities.end(); ++i ){
        i->second->Update(frameTime);
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



// To Send a snapshot via Socket Connection
sf::Packet &operator<<(sf::Packet& packet, const World& world){

    // Only the Server needs m_entity_index, as he gives new objects
    // their names...

    // frameClock is individually for every client

    // Send number of entites
    packet << (sf::Uint16)(world.mEntities.size());

    // Iterate through all Entities and send their keys and them
    for (EntityMap::const_iterator i = world.mEntities.begin(); i!=world.mEntities.end(); ++i ){
        packet << (sf::Uint16)(i->first);
        //packet << (*i->second);
    }

    return packet ;
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

        //IEntity* entity;
        //packet >> (*entity);

        // TODO: add it
    }

    return packet;
}
