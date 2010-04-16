#include "PowerGenerator.hpp"
#include <iostream>

PowerGenerator::PowerGenerator() {
    mImageKey = "generator";
    mAnimationFPS = 3;
    mAnimationIndex = 0;
    mPosX = 0;
    mPosY = 0;
}

void PowerGenerator::Update(float blocksize, float dt) {
    // Update Life Time
    mLifeTime += dt;

    // Set Image Pointer before Animation, as setting the Image changes the Subrect
    mSprite.SetImage(* ResourceManager::get_mutable_instance().GetImage(mImageKey));

    // Update Animation
    UpdateAnimation(blocksize);
}

sf::Uint16 PowerGenerator::GetPower() {
    return mPower;
}

void PowerGenerator::SetPosition(sf::Vector2f pos) {
    mPosX = pos.x+0.25f;
    mPosY = pos.y+0.5f;
}
