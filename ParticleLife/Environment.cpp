#include "Environment.h"
#include <cassert>

#define RADIUS 5

Environment::Environment(unsigned int width, unsigned int height)
{
	m_width = static_cast<float>(width);
	m_height = static_cast<float>(height);
	m_boundWidth = m_width;
	m_boundHeight = m_height;
	m_gen.seed(m_rd());
	m_bndColTy = BoundaryCollisionType::EMPTY;
}

void Environment::update()
{
	const float r = static_cast<float>(RADIUS);

	for (unsigned int i = 0; i < m_particleCount; i++)
	{
		Particle& p = m_particles[i];
		p.x += p.vx;
		p.y += p.vy;

		switch (m_bndColTy)
		{
		case BoundaryCollisionType::EMPTY:
			// Boundaries are ignored
			break;
		case BoundaryCollisionType::SOLID:
			if (p.x < r)             { p.x = r;             p.vx *= -1.0f; }
			if (p.y < r)             { p.y = r;             p.vy *= -1.0f; }
			if (p.x > m_boundWidth)  { p.x = m_boundWidth;  p.vx *= -1.0f; }
			if (p.y > m_boundHeight) { p.y = m_boundHeight; p.vy *= -1.0f; }
			break;
		case BoundaryCollisionType::WRAP:
			if (p.x < -r)            { p.x =  m_boundWidth;	 }
			if (p.y < -r)            { p.y =  m_boundHeight; }
			if (p.x > m_boundWidth)  { p.x = -r;             }
			if (p.y > m_boundHeight) { p.y = -r;             }
			break;
		}
	}
}

void Environment::draw(sf::RenderWindow * window)
{
	sf::CircleShape shape(static_cast<float>(RADIUS), 11);
	shape.setOrigin(static_cast<float>(RADIUS), static_cast<float>(RADIUS));
	shape.setOutlineColor(sf::Color::Transparent);

	for (unsigned int i = 0; i < m_particleCount; i++)
	{
		Particle& p = m_particles[i];
		shape.setPosition(p.x, p.y);
		shape.setFillColor(m_types.color(p.type));
		window->draw(shape);
	}
}

void Environment::createRandomParticles(std::size_t particleCount, float velMean, float velStd)
{
	m_particleCount = particleCount;
	
	int maxColorIndex = static_cast<int>(m_types.size()) - 1;
	assert(maxColorIndex > -1); // If you trigger the assertion evaluation make sure you created types before creating particles
	std::uniform_int_distribution<unsigned int> randCol(0, static_cast<unsigned int>(maxColorIndex));

	std::uniform_real_distribution<float> randUniPosW(0.0f, static_cast<float>(m_width));
	std::uniform_real_distribution<float> randUniPosH(0.0f, static_cast<float>(m_height));

	std::normal_distribution<float> randNormVel(velMean, velStd);

	for (unsigned i = 0; i < particleCount; i++)
	{
		m_particles.emplace_back(randUniPosW(m_gen), randUniPosH(m_gen), randNormVel(m_gen), randNormVel(m_gen), (uint8_t)(randCol(m_gen)));
	}
}

void Environment::createRandomTypes(std::size_t typeCount)
{
	m_types.addRandomTypes(typeCount);
}

void Environment::setBoundaryCollisionType(BoundaryCollisionType bndColTy)
{
	m_bndColTy = bndColTy;

	switch (bndColTy)
	{
	case BoundaryCollisionType::EMPTY:
		// Bounds are the same size as the environment size since we ignore them anyway
		m_boundWidth =  m_width;
		m_boundHeight = m_height;
		break;
	case BoundaryCollisionType::SOLID:
		// Bounds are the environment size minus the balls radius to make sure the balls bounce while
		// they are still onscreen
		m_boundWidth =  m_width  - static_cast<float>(RADIUS);
		m_boundHeight = m_height - static_cast<float>(RADIUS);
		break;
	case BoundaryCollisionType::WRAP:
		// Bounds are the environment size plus the balls radius to make sure the wrapping happens
		// when the balls are offscreen
		m_boundWidth =  m_width  + static_cast<float>(RADIUS);
		m_boundHeight = m_height + static_cast<float>(RADIUS);
		break;
	}
}
