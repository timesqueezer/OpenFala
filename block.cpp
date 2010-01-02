#include "block.hpp"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

Block::Block(float x, float y, sf::Image *Image) {
	Sprite.SetImage(*Image);
	Sprite.SetPosition(x, y);
}
