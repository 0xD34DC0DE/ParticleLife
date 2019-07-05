#pragma once
#include <SFML/Graphics.hpp>
#include "TextureRenderer.h"
#include <vector>

class BatchRenderer2D
{
public:
	BatchRenderer2D();

	unsigned int addTexture(const sf::CircleShape& circleShape, unsigned int textureSize, sf::Color backgroundColor = sf::Color::Transparent);

	unsigned int addSprite(float x, float y, float quadSize, int textureIndex = -1);

	void draw(sf::RenderTarget* renderTarget);

	void drawAtlas(sf::RenderTarget * renderTarget, float x, float y, float width, float height);

	void setPos(std::size_t index, float x, float y);

private:
	sf::VertexBuffer m_vertexBuffer;
	sf::Texture m_textureAtlas;
	unsigned int m_textureCount;
	unsigned int m_spriteCount;
	std::vector<sf::Vertex> m_vertices;
	std::vector<float> m_quadSizes;
	bool m_vertexBufferNeedsUpdate;
	sf::RenderStates m_renderState;
	std::vector<unsigned int> m_textureSizes;
	std::vector<float> m_texturesUVOffsets;
};

