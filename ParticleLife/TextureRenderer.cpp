#include "TextureRenderer.h"



TextureRenderer::TextureRenderer()
{
}

sf::Image TextureRenderer::renderToTexture(const sf::CircleShape& circleShape, unsigned int textureSize, sf::Color backgoundColor)
{
	sf::CircleShape circleShapeCopy(circleShape);

	// Set the circle radius to fill all the space available
	const float sqrt2 = std::sqrtf(2.0f);
	float r = sqrt2 * textureSize;
	circleShapeCopy.setRadius(r);

	// Center the circle's origin
	circleShapeCopy.setOrigin(r, r);

	// Set the circle's position to the middle of the render texture


	m_renderTextureBuffer.create(textureSize, textureSize);
	m_renderTextureBuffer.clear(backgoundColor);

	m_renderTextureBuffer.draw(circleShapeCopy);

	return m_renderTextureBuffer.getTexture().copyToImage();
}
