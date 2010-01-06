#ifndef BLOCK_HPP_INCLUDED
#define BLOCK_HPP_INCLUDED

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class Block {
public:
    Block(float, float, sf::Image*);
    sf::Sprite Sprite;
    void SetPos(int, int);
};

#endif
