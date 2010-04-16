#include "CannonTurret.hpp"

CannonTurret::CannonTurret() {
    mAnimationFPS = 1;
    mAnimationIndex = 0;
    mPosX = 0;
    mPosY = 0;
    mRotation = 0;
}

void CannonTurret::Update(float dt) {
    
    
    mLifeTime += dt;

    

    mSprite.SetImage(* ResourceManager::get_mutable_instance().GetImage(mImageKey));
    
    Rotate(10*dt);

    UpdateAnimation();

    
    mSprite.SetCenter(mDimX/2, mDimY/2);
    mSprite.SetRotation(mRotation);

    
}

void CannonTurret::SetRotation(float angle) {
    mRotation = angle;
}

void CannonTurret::Rotate(float angle) {
    mRotation += angle;
}

float CannonTurret::GetRotation() {
    return mRotation;
}
