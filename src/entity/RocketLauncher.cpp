#include "RocketLauncher.hpp"

RocketLauncher::RocketLauncher() {
    mAnimationFPS = 1;
    mAnimationIndex = 0;
    mPosX = 0;
    mPosY = 0;
    direction = false;
}

void RocketLauncher::Update(float dt) {
    
    
    mLifeTime += dt;

    

    mSprite.SetImage(* ResourceManager::get_mutable_instance().GetImage(mImageKey));

    UpdateAnimation();

}
