#ifndef BUILDING_HPP_INCLUDED
#define BUILDING_HPP_INCLUDED

#include "IEntity.hpp"

class Building : public IEntity{
public:
    Building();
    void Update(float blocksize, float dt);

    void SetHealth(sf::Uint16 health);
    sf::Uint16 GetHealth();

    friend sf::Packet &operator<<(sf::Packet& packet, const Building& building);
    friend sf::Packet &operator>>(sf::Packet& packet, Building& building);

private:
    static const sf::Uint16 mCost = 1000;
    sf::Uint16 mHealth;

};

#endif
