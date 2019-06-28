#include "TextureRenderer.h"



TextureRenderer::TextureRenderer()
{
}

sf::Image TextureRenderer::renderToTexture(const sf::CircleShape& circleShape, unsigned int textureSize, sf::Color backgoundColor)
{
	sf::CircleShape circleShapeCopy(circleShape);
	// Middle of the texture (square texture)
	float pos = (static_cast<float>(textureSize) / 2.0f);

	// Substract the outline thickness so it isn't rendered outside the bounds of the texture
	float radius = pos - circleShapeCopy.getOutlineThickness();
	
	// Set the circle radius to fill all the space available
	circleShapeCopy.setRadius(radius);

	// Center the circle's origin
	circleShapeCopy.setOrigin(radius, radius);

	// Set the circle's position to the middle of the render texture
	circleShapeCopy.setPosition(pos, pos);

	sf::RenderTexture rtBuffer;
	rtBuffer.create(textureSize, textureSize);
	rtBuffer.clear(backgoundColor);

	rtBuffer.draw(circleShapeCopy);

	return rtBuffer.getTexture().copyToImage();
}
