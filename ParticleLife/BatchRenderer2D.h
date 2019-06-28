#pragma once
#include <SFML/Graphics.hpp>
#include "TextureRenderer.h"
#include <vector>

class BatchRenderer2D
{
public:
	BatchRenderer2D();

	unsigned int add(const sf::CircleShape& circleShape, unsigned int textureSize, float quadSize, sf::Color backgroundColor = sf::Color::Transparent);

	void draw(sf::RenderTarget* renderTarget);

	void drawAtlas(sf::RenderTarget * renderTarget, float x, float y, float width, float height);

	void setPos(std::size_t index, float x, float y);

private:
	sf::VertexBuffer m_vertexBuffer;
	sf::Texture m_textureAtlas;
	unsigned int m_textureCount;
	std::vector<sf::Vertex> m_vertices;
	std::vector<float> m_quadSizes;
	bool m_vertexBufferNeedsUpdate;
	sf::RenderStates m_renderState;
};

