#include "Building.hpp"
#include <iostream>

Building::Building(){
    mAnimationFPS = 1;
    mAnimationIndex = 0;
    mPosX = 0;
    mPosY = 0;
}

void Building::Update(float dt){
    // Update Life Time and Animation
    mLifeTime += dt;
    UpdateAnimation();

    // Destroy Building after 5 seconds (for testing purposes)
    if (mLifeTime > 5 and mAnimationIndex!=1){
        mAnimationIndex = 1;
    }
}

void Building::SetHealth(sf::Uint16 health){
    mHealth = health;
}

sf::Uint16 Building::GetHealth(){
    return mHealth;
}
