#include "block.hpp"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

Block::Block(float x, float y, sf::Image *Image, unsigned short type) {
	Sprite.SetImage(*Image);
	Sprite.SetPosition(x, y);
	m_type = type;
}

void Block::SetPos(int x, int y) {
	Sprite.SetPosition(x, y);
}
