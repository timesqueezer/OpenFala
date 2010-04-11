#include "block.hpp"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

Block::Block(float x, float y, sf::Image *Image, BlockType type) {
	Sprite.SetImage(*Image);
	Sprite.SetPosition(x, y);
	m_type = type;
}

Block::Block(float x, float y, sf::Shape Shape) {
    m_Shape = Shape;
    Shape.SetPosition(x, y);
    m_type = BLOCKTYPE_GROUND;
}

void Block::SetPos(int x, int y) {
	Sprite.SetPosition(x, y);
}
