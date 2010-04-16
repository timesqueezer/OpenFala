#ifndef POWERGENERATOR_HPP_INCLUDED
#define POWERGENERATOR_HPP_INCLUDED

#include "IEntity.hpp"

class PowerGenerator : public IEntity{
public:
    PowerGenerator();
    void Update(float blocksize, float dt);

    sf::Uint16 GetPower();

    void SetPosition(sf::Vector2f pos);

private:
    static const sf::Uint16 mCost = 1000;
    static const sf::Uint16 mPower = 200;
};

#endif
