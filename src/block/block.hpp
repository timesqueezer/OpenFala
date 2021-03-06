#ifndef BLOCK_HPP_INCLUDED
#define BLOCK_HPP_INCLUDED

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

enum BlockType{
    BLOCKTYPE_GROUND,
    BLOCKTYPE_EMPTY,
    BLOCKTYPE_TOWER
};

// Generic class to represent a block in the game world.
class Block {
public:
    Block(float x, float y, sf::Image* Image, BlockType type);

    // TODO: ganz hässlich!!!
    sf::Sprite Sprite;

    BlockType m_type;
    void SetPos(int, int);
};

#endif
