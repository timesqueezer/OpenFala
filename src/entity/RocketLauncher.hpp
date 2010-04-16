#ifndef ROCKETLAUNCHER_HPP_INCLUDED
#define ROCKETLAUNCHER_HPP_INCLUDED

#include "IEntity.hpp"

class RocketLauncher : public IEntity {
public:
    RocketLauncher();
    void Update(float dt);

private:
    static const sf::Uint16 mCost = 1000;
    bool direction; // 0 for left and 1 for right;
};

#endif
