#include "CannonTurret.hpp"

CannonTurret::CannonTurret() {
    mAnimationFPS = 1;
    mAnimationIndex = 0;
    mPosX = 0;
    mPosY = 0;
    mRotation = 0;
}

void CannonTurret::Update(float blocksize, float dt) {


    mLifeTime += dt;



    mSprite.SetImage(* ResourceManager::get_mutable_instance().GetImage(mImageKey));

    Rotate(20*dt);

    UpdateAnimation(blocksize);


    mSprite.SetCenter(mDimX * blocksize / 2, mDimY * blocksize / 2);
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
