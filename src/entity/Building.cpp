#include "Building.hpp"
#include <iostream>

Building::Building() {
    mImageKey = "building";
    mType = ENTITY_TYPE_BUILDING;
    mAnimationFPS = 1;
    mAnimationIndex = 0;
    mPosX = 0;
    mPosY = 0;
}

void Building::Update(float blocksize, float dt) {
    // Update Life Time
    mLifeTime += dt;

    // Set Image Pointer before Animation, as setting the Image changes the Subrect
    mSprite.SetImage(* ResourceManager::get_mutable_instance().GetImage(mImageKey));

    // Update Animation
    UpdateAnimation(blocksize);

    // Destroy Building after 10 seconds (for testing purposes)
    if (mLifeTime > 10 and mAnimationIndex!=1) {
        mAnimationIndex = 1;
    }
}

void Building::SetHealth(sf::Uint16 health) {
    mHealth = health;
}

sf::Uint16 Building::GetHealth() {
    return mHealth;
}



sf::Packet &operator<<(sf::Packet& packet, const Building& building) {
    building.SerializeIntoPacket(packet);
    packet << building.mHealth;
    return packet;
}

sf::Packet &operator>>(sf::Packet& packet, Building& building) {
    building.SerializeFromPacket(packet);
    packet >> building.mHealth;
    return packet;
}
