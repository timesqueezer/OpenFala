#ifndef CANNONTURRET_HPP_INCLUDED
#define CANNONTURRET_HPP_INCLUDED

#include "IEntity.hpp"

class CannonTurret : public IEntity {
public:
    CannonTurret();
    void Update(float dt);
    void SetRotation(float angle);
    void Rotate(float angle);
    float GetRotation();

private:
    static const sf::Uint16 mCost = 1000;
    float mRotation;
    
};

#endif
