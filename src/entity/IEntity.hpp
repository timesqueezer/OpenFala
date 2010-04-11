#ifndef IENTITY_HPP_INCLUDED
#define IENTITY_HPP_INCLUDED

#include <boost/ptr_container/ptr_container.hpp>

#include <SFML/Graphics.hpp>

class IEntity : boost::noncopyable {
public:
    virtual ~IEntity() {}
    virtual void Update(float dt) = 0;

    sf::Uint16 GetAnimationIndex();
    void SetAnimationIndex(sf::Uint16 index);

    float GetLifeTime();

    sf::Vector2f GetPosition();
    void SetPosition(sf::Vector2f position);
    void SetX(float x);
    void SetY(float y);

    sf::Vector2f GetDimension();
    void SetDimension(sf::Vector2f dimension);

    float GetMass();
    void SetMass(float mass);

    sf::Uint16 GetPlayerID();
    void SetPlayerID(sf::Uint16 id);

    sf::Uint16 GetAnimationFPS();
    void SetAnimationFPS(sf::Uint16 FPS);

private:
    sf::Sprite mSprite;
    sf::Uint16 mAnimationIndex;
    sf::Uint16 mAnimationFPS;
    float mLifeTime;
    float mPosX;
    float mPosY;
    float mDimX;
    float mDimY;
    float mMass;
    sf::Uint16 mPlayerID;

    virtual void UpdateAnimation();
};

#endif
