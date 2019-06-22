#pragma once
#include <SFML/Graphics.hpp>

class TextureRenderer
{
public:
	TextureRenderer();

	static sf::Image renderToTexture(const sf::CircleShape& circleShape, unsigned int textureSize, sf::Color backgroundColor = sf::Color::Transparent);
private:
};

