#include "BatchRenderer2D.h"


BatchRenderer2D::BatchRenderer2D()
{
	m_vertexBuffer.create(1);
	m_vertexBuffer.setPrimitiveType(sf::PrimitiveType::Quads);
	m_vertexBufferNeedsUpdate = true;
	m_renderState = sf::RenderStates(&m_textureAtlas);
	m_textureCount = 0;
	m_spriteCount = 0;
}

// Returns the ID of the sprite
//TODO : Make it so we can batch our particles when adding them
unsigned int BatchRenderer2D::addTexture(const sf::CircleShape& circleShape, unsigned int textureSize, sf::Color backgroundColor) 
{
	sf::Texture tex;
	tex.create(textureSize, textureSize);
	tex.loadFromImage(TextureRenderer::renderToTexture(circleShape, textureSize, backgroundColor));
	return addTexture(tex);
}

unsigned int BatchRenderer2D::addTexture(const sf::Texture & texture)
{
	m_textureCount++;

	sf::Vector2u textureDimension = texture.getSize();
	unsigned int textureSize = std::max(textureDimension.x, textureDimension.y);

	// Store the texture offset so sprites can refer to it later without linear search
	m_texturesUVOffsets.emplace_back(static_cast<float>((m_textureAtlas.getSize().x)));

	sf::Texture newAtlas;

	// Set the new Atlas's height to the texture size if its bigger
	unsigned int newAtlasHeight = (m_textureAtlas.getSize().y < textureSize) ? textureSize : m_textureAtlas.getSize().y;

	// Create a new texture width the required additionnal space (can't resize an existing texture we need to create a new one)
	newAtlas.create(m_textureAtlas.getSize().x + textureSize, newAtlasHeight);

	// Copy the old texture atlas into the new one
	newAtlas.update(m_textureAtlas);

	// Copy the texture into the new atlas and copy the atlas into the new atlas
	newAtlas.update(texture, m_textureAtlas.getSize().x, 0);

	// Swap the old atlas for new one
	m_textureAtlas.swap(newAtlas);

	// Store the texture size so we know how big the quad is
	m_textureSizes.emplace_back(textureSize);

	//return the current texture index
	return m_textureCount - 1;
}

void BatchRenderer2D::modifyTexture(const sf::CircleShape& circleShape, unsigned int textureIndex, sf::Color backgroundColor)
{
	sf::Texture tex;
	unsigned int texSize = m_textureSizes[textureIndex];
	tex.create(texSize, texSize);
	tex.loadFromImage(TextureRenderer::renderToTexture(circleShape, texSize, backgroundColor));
	m_textureAtlas.update(tex, m_texturesUVOffsets[textureIndex], 0);
}

unsigned int BatchRenderer2D::addSprite(float x, float y, float quadSize, int textureIndex)
{
	m_spriteCount++;

	m_quadSizes.emplace_back(quadSize);
	m_spritesActiveTexture.emplace_back(textureIndex);

	float uvOffset = 0.0f;
	float textureSize = 0.0f;

	if (textureIndex != -1)
	{
		uvOffset = m_texturesUVOffsets[textureIndex];
		textureSize = m_textureSizes[textureIndex];
	}
		
	// create new vertices for the quad with the correct UV coordinates
	m_vertices.emplace_back(sf::Vertex(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(uvOffset, 0.0f)));
	m_vertices.emplace_back(sf::Vertex(sf::Vector2f(quadSize, 0.0f), sf::Vector2f(uvOffset + textureSize, 0.0f)));
	m_vertices.emplace_back(sf::Vertex(sf::Vector2f(quadSize, quadSize), sf::Vector2f(uvOffset + textureSize, textureSize)));
	m_vertices.emplace_back(sf::Vertex(sf::Vector2f(0.0f, quadSize), sf::Vector2f(uvOffset, textureSize)));

	// Invalidate the current vertex buffer since we modified the content and it needs to be updated inside the GPU
	m_vertexBufferNeedsUpdate = true;

	// return current sprite index
	return m_spriteCount - 1;
}


void BatchRenderer2D::draw(sf::RenderTarget * renderTarget)
{
	if (m_vertexBufferNeedsUpdate)
	{
		m_vertexBuffer.update(m_vertices.data(), m_vertices.size(), 0);
		m_vertexBufferNeedsUpdate = false;
	}

	//TODO : implement the option to set a custom shader(bloom ?)
	renderTarget->draw(m_vertexBuffer, m_renderState);
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

void BatchRenderer2D::setPos(std::size_t index, float x, float y, bool useTextureSizeAsQuadSize)
{
	float size;
	if (useTextureSizeAsQuadSize)
		size = m_textureSizes[m_spritesActiveTexture[index]] / 2.0f;
	else
		size = m_quadSizes[index];

	m_vertices[index * 4].position.x = x - size;
	m_vertices[index * 4].position.y = y - size;

	m_vertices[index * 4 + 1].position.x = x + size;
	m_vertices[index * 4 + 1].position.y = y - size;

	m_vertices[index * 4 + 2].position.x = x + size;
	m_vertices[index * 4 + 2].position.y = y + size;

	m_vertices[index * 4 + 3].position.x = x - size;
	m_vertices[index * 4 + 3].position.y = y + size;

	// Invalidate the current vertex buffer since we modified the content and it needs to be updated inside the GPU
	m_vertexBufferNeedsUpdate = true;
}

void BatchRenderer2D::setSpriteTexture(std::size_t spriteIndex, std::size_t textureIndex)
{
	float offset = m_texturesUVOffsets[textureIndex];
	float size = m_textureSizes[textureIndex];

	m_vertices[spriteIndex * 4].texCoords.x = offset;

	m_vertices[spriteIndex * 4 + 1].texCoords.x = offset + size;

	m_vertices[spriteIndex * 4 + 2].texCoords.x = offset + size;
	m_vertices[spriteIndex * 4 + 2].texCoords.y = size;

	m_vertices[spriteIndex * 4 + 3].texCoords.x = offset;
	m_vertices[spriteIndex * 4 + 3].texCoords.y = size;

	m_spritesActiveTexture[spriteIndex] = textureIndex;
}
