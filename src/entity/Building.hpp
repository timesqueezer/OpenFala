#ifndef BUILDING_HPP_INCLUDED
#define BUILDING_HPP_INCLUDED

#include "IEntity.hpp"

class Building : public IEntity{
public:
    Building();
    void Update(float dt);

    void SetHealth(sf::Uint16 health);
    sf::Uint16 GetHealth();
private:
    static const sf::Uint16 mCost = 1000;
    sf::Uint16 mHealth;

};

#endif
