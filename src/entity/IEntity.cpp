#include "IEntity.hpp"

#include <iostream>

sf::Uint16 IEntity::GetType() const {
    return mType;
}

sf::Uint16 IEntity::GetAnimationIndex(){
    return mAnimationIndex;
}

sf::Sprite& IEntity::GetSprite(float blockSize, sf::Vector2f offset){
    mSprite.SetPosition(mPosX * blockSize + offset.x, mPosY * blockSize + offset.y);
    return mSprite;
}

void IEntity::SetAnimationIndex(sf::Uint16 index){
    mAnimationIndex = index;
}

float IEntity::GetLifeTime(){
    return mLifeTime;
}

sf::Vector2f IEntity::GetPosition() const {
    return sf::Vector2f(mPosX, mPosY);
}

void IEntity::SetPosition(sf::Vector2f position) {
    mPosX = position.x;
    mPosY = position.y;
}

void IEntity::SetX(const float x){
    mPosX = x;
}

void IEntity::SetY(const float y){
    mPosY = y;
}

sf::Vector2f IEntity::GetDimension() const {
    return sf::Vector2f(mDimX, mDimY);
}

void IEntity::SetDimension(const sf::Vector2f dimension) {
    mDimX = dimension.x;
    mDimY = dimension.y;
}

float IEntity::GetMass() const {
    return mMass;
}

void IEntity::SetMass(float mass){
    mMass = mass;
}

sf::Uint16 IEntity::GetPlayerID() const {
    return mPlayerID;
}

void IEntity::SetPlayerID(const sf::Uint16 id){
    mPlayerID = id;
}

sf::Uint16 IEntity::GetAnimationFPS() const {
    return mAnimationFPS;
}
void IEntity::SetAnimationFPS(sf::Uint16 FPS){
    mAnimationFPS = FPS;
}

void IEntity::UpdateAnimation(float blocksize) {
    // Get Length of Animation in Frames
    sf::Uint16 framesPerLoop = mSprite.GetImage()->GetWidth() / (mDimX*blocksize);

    // Get number of frames since start
    sf::Uint16 frameNb = (mLifeTime * mAnimationFPS);

    // Get number of frames since last loop start
    if (framesPerLoop!=0)
        frameNb = frameNb % framesPerLoop;
    else
        frameNb = 0;

    // x-Coordinate = frame Number * frame Width
    sf::Uint16 imageX =  frameNb * mDimX * blocksize ;

    // y-Coordinate = row Number * frame Height
    sf::Uint16 imageY = mAnimationIndex * mDimY * blocksize;

    // Set Subrect
    mSprite.SetSubRect( sf::Rect<int>(imageX, imageY, imageX+mDimX*blocksize, imageY+mDimY*blocksize) );
}

void IEntity::SerializeIntoPacket(sf::Packet& packet) const {
    packet << mAnimationIndex;
    packet << mAnimationFPS;
    packet << mLifeTime;
    packet << mPosX;
    packet << mPosY;
    packet << mDimX;
    packet << mDimY;
    packet << mMass;
    packet << mPlayerID;
}

void IEntity::SerializeFromPacket(sf::Packet& packet) {
    packet >> mAnimationIndex;
    packet >> mAnimationFPS;
    packet >> mLifeTime;
    packet >> mPosX;
    packet >> mPosY;
    packet >> mDimX;
    packet >> mDimY;
    packet >> mMass;
    packet >> mPlayerID;
}

sf::Packet &operator<<(sf::Packet& packet, IEntity& entity) {
    return packet;
}
sf::Packet &operator>>(sf::Packet& packet, IEntity& entity) {
    return packet;
}
