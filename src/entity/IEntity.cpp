#include "IEntity.hpp"

sf::Uint16 IEntity::GetAnimationIndex()
    return mAnimationIndex;

void IEntity::SetAnimationIndex(sf::Uint16 index)
    mAnimationIndex = index;

float IEntity::GetLifeTime()
    return mLifeTime;

sf::Vector2f IEntity::GetPosition()
    return sf::Vector2f(mPosX, mPosY);

void IEntity::SetPosition(sf::Vector2f position) {
    mPosX = position.X; 
    mPosY = position.Y;
}

void IEntity::SetX(float x)
    mPosX = x;

void IEntity::SetY(float y)
    mPosY = y;

sf::Vector2f IEntity::GetDimension()
    return sf::Vector2f(mDimX, mDimY);

void IEntity::SetDimension(sf::Vector2f dimension) {
    mDimX = dimension.X;
    mDimY = dimension.Y;
}

float IEntity::GetMass()
    return mMass;

void IEntity::SetMass(float mass)
    mMass = mass;

sf::Uint16 IEntity::GetPlayerID()
    return mPlayerID;

void IEntity::SetPlayerID(sf::Uint16 id)
    mPlayerID = id;

void IEntity::UpdateAnimation() {
    sf::Uint16 imageY = mAnimationIndex * mSprite.GetHeight();
    sf::Uint16 imageX = 0;
}
