#include "RocketLauncher.hpp"

RocketLauncher::RocketLauncher() {
    mImageKey = "turret2";
    mAnimationFPS = 1;
    mAnimationIndex = 0;
    mPosX = 0;
    mPosY = 0;
    direction = false;
}

void RocketLauncher::Update(float blocksize, float dt) {

    mLifeTime += dt;
    mSprite.SetImage(* ResourceManager::get_mutable_instance().GetImage(mImageKey));
    UpdateAnimation(blocksize);

}
