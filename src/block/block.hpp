#ifndef BLOCK_HPP_INCLUDED
#define BLOCK_HPP_INCLUDED

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

// Generic class to represent a block in the game world.
class Block {
public:
    Block(float, float, sf::Image*, unsigned short);

    Block(float x, float y, sf::Shape Shape);
    sf::Sprite Sprite;
    sf::Shape m_Shape;
    unsigned short m_type; // type 0 for ground type 1 for sky and type 2 for tower
    void SetPos(int, int);
};

#endif
