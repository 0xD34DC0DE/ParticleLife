#include "BatchRenderer2D.h"



BatchRenderer2D::BatchRenderer2D()
{
	m_vertexBuffer.create(1);
	m_vertexBuffer.setPrimitiveType(sf::PrimitiveType::Quads);
}

// Returns the ID of the sprite
unsigned int BatchRenderer2D::add(const sf::CircleShape& circleShape, unsigned int textureSize, sf::Color backgroundColor) 
{
	sf::Vector2u newQuadUVOffsets = m_textureAtlas.getSize();
	sf::Vector2f newQuadUVOffsetsFloat(static_cast<float>(newQuadUVOffsets.x), static_cast<float>(newQuadUVOffsets.y));

	const float textureSizeFloat = static_cast<float>(textureSize);

	// create new vertices for the quad with the correct UV coordinates
	m_vertices.emplace_back(sf::Vertex(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(newQuadUVOffsets.x, 0.0f)));
	m_vertices.emplace_back(sf::Vertex(sf::Vector2f(textureSizeFloat, 0.0f), sf::Vector2f(newQuadUVOffsets.x + textureSizeFloat, 0.0f)));
	m_vertices.emplace_back(sf::Vertex(sf::Vector2f(textureSizeFloat, textureSizeFloat), sf::Vector2f(newQuadUVOffsets.x + textureSizeFloat, textureSizeFloat)));
	m_vertices.emplace_back(sf::Vertex(sf::Vector2f(0.0f, textureSizeFloat), sf::Vector2f(newQuadUVOffsets.x, textureSizeFloat)));

	// Update the vertices of the vertex buffer
	m_vertexBuffer.update(m_vertices.data(), m_vertices.size(), 0);

	sf::Texture newAtlas;

	// Set the new Atlas's height to the texture size if its bigger
	unsigned int newAtlasHeight = (m_textureAtlas.getSize().y < textureSize) ? textureSize : m_textureAtlas.getSize().y;

	// Create a new texture width the required additionnal space (can't resize an existing texture)
	newAtlas.create(m_textureAtlas.getSize().x + textureSize, newAtlasHeight);

	// Copy the old texture atlas into the new one
	newAtlas.update(m_textureAtlas);
	
	// Draw the circle and copy it into the new atlas
	newAtlas.update(TextureRenderer::renderToTexture(circleShape, textureSize, backgroundColor), m_textureAtlas.getSize().x, 0); 

	// Swap the old atlas for new one
	m_textureAtlas.swap(newAtlas);

	return (m_vertices.size() / 4) - 1;
}

void BatchRenderer2D::draw(sf::RenderTarget * renderTarget)
{

}

// Draws the whole texture atlas
void BatchRenderer2D::drawAtlas(sf::RenderTarget * renderTarget, float x, float y, float width, float height)
{
	sf::RectangleShape rect(sf::Vector2f(width, height));
	rect.setOrigin(width / 2.0f, height / 2.0f);
	rect.setPosition(x, y);
	rect.setTexture(&m_textureAtlas);
	renderTarget->draw(rect);
	rect.setTexture(NULL); // Destroy the reference to the texture
}
