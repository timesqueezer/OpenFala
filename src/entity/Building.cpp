#include "Building.hpp"

Building::Building(){
    SetAnimationFPS(1);
}

void Building::Update(float dt){
    // Update Life Time and Animation
    mLifeTime += dt;
    UpdateAnimation();

    // Destroy Building after 5 seconds (for testing purposes)
    if (dt > 5 and mAnimationIndex!=1)
        SetAnimationIndex(1);
}

void Building::SetHealth(sf::Uint16 health){
    mHealth = health;
}

sf::Uint16 Building::GetHealth(){
    return mHealth;
}
