#include "World.hpp"
#include <boost/foreach.hpp>
#include <iostream>

void World::AddEntity(IEntity* entity) {
    mEntities.insert(++m_entity_index, entity);
}

boost::ptr_map<sf::Uint16, IEntity>& World::GetEntities() {
    return mEntities;
}


// Calls all Entity Update methods, calculates physics
void World::Update(){

    // reset frame Clock before frame Calculations, but remember time
    float frameTime = frameClock.GetElapsedTime();
    frameClock.Reset();


    for ( EntityMap::iterator i = mEntities.begin(); i!=mEntities.end(); ++i ){
        i->second->Update(frameTime);
    }

}

// Draws all Entities to RenderTarget
void World::Draw(sf::RenderTarget& target, float blocksize, sf::Vector2f offset){

    for ( EntityMap::iterator i = mEntities.begin(); i!=mEntities.end(); ++i ){
        sf::Sprite& s = i->second->GetSprite(blocksize, offset);
        target.Draw(s);
    }
}
