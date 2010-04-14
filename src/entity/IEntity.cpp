#include "IEntity.hpp"

#include <iostream>

sf::Uint16 IEntity::GetAnimationIndex(){
    return mAnimationIndex;
}

sf::Sprite& IEntity::GetSprite(float blockSize){
    mSprite.SetPosition(mPosX*blockSize, mPosY*blockSize);
    return mSprite;
}

void IEntity::SetImage(sf::Image* img){
    mSprite.SetImage(*img);
}

void IEntity::SetAnimationIndex(sf::Uint16 index){
    mAnimationIndex = index;
}

float IEntity::GetLifeTime(){
    return mLifeTime;
}

sf::Vector2f IEntity::GetPosition(){
    return sf::Vector2f(mPosX, mPosY);
}

void IEntity::SetPosition(sf::Vector2f position) {
    mPosX = position.x;
    mPosY = position.y;
}

void IEntity::SetX(float x){
    mPosX = x;
}

void IEntity::SetY(float y){
    mPosY = y;
}

sf::Vector2f IEntity::GetDimension(){
    return sf::Vector2f(mDimX, mDimY);
}

void IEntity::SetDimension(sf::Vector2f dimension) {
    mDimX = dimension.x;
    mDimY = dimension.y;
}

float IEntity::GetMass(){
    return mMass;
}

void IEntity::SetMass(float mass){
    mMass = mass;
}

sf::Uint16 IEntity::GetPlayerID(){
    return mPlayerID;
}

void IEntity::SetPlayerID(sf::Uint16 id){
    mPlayerID = id;
}

sf::Uint16 IEntity::GetAnimationFPS(){
    return mAnimationFPS;
}
void IEntity::SetAnimationFPS(sf::Uint16 FPS){
    mAnimationFPS = FPS;
}


void IEntity::UpdateAnimation() {
    // Get Length of Animation in Frames
    sf::Uint16 framesPerLoop = mSprite.GetImage()->GetWidth() / mDimX;

    // Get number of frames since start
    sf::Uint16 frameNb = (mLifeTime * mAnimationFPS);

    // Get number of frames since last loop start
    if (framesPerLoop!=0)
        frameNb = frameNb % framesPerLoop;
    else
        frameNb = 0;

    // x-Coordinate = frame Number * frame Width
    sf::Uint16 imageX =  frameNb * mDimX;

    // y-Coordinate = row Number * frame Height
    sf::Uint16 imageY = mAnimationIndex * mDimY;

    std::cout << "Subrect: " << imageX << " | " << imageY << " | " << imageX+mDimX << " | " << imageY+mDimY << std::endl;
    // Set Subrect
    mSprite.SetSubRect( sf::Rect<int>(imageX, imageY, imageX+mDimX, imageY+mDimY) );
}
