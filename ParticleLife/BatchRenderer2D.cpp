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

	sf::VertexBuffer newVertexBuffer;
	newVertexBuffer.create(m_vertexBuffer.getVertexCount() + 4); // Create new buffer with additional memory for 4 vertex

	const float textureSizeFloat = static_cast<float>(textureSize);

	// create new vertices for the quad with the correct UV coordinates
	m_vertices.emplace_back(sf::Vertex(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(newQuadUVOffsets.x, 0.0f)));
	m_vertices.emplace_back(sf::Vertex(sf::Vector2f(textureSizeFloat, 0.0f), sf::Vector2f(newQuadUVOffsets.x + textureSizeFloat, 0.0f)));
	m_vertices.emplace_back(sf::Vertex(sf::Vector2f(textureSizeFloat, textureSizeFloat), sf::Vector2f(newQuadUVOffsets.x + textureSizeFloat, textureSizeFloat)));
	m_vertices.emplace_back(sf::Vertex(sf::Vector2f(0.0f, textureSizeFloat), sf::Vector2f(newQuadUVOffsets.x, textureSizeFloat)));


	//TODO : continue from here -> implement copying into the vertex buffer the vertices

	TextureRenderer::renderToTexture(circleShape, textureSize, backgroundColor);
}

void BatchRenderer2D::draw(sf::RenderTarget * renderTarget)
{

}
