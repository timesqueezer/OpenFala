#ifndef BLOCK_HPP_INCLUDED
#define BLOCK_HPP_INCLUDED

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

// Generic class to represent a block in the game world.
class Block {
public:
    Block(float, float, sf::Image*);
    sf::Sprite Sprite;
    void SetPos(int, int);
};

#endif
