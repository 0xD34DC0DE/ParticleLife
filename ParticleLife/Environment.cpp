#include "Environment.h"
#include <cassert>

#define RADIUS 5

Environment::Environment(unsigned int width, unsigned int height)
{
	m_width = static_cast<float>(width);
	m_height = static_cast<float>(height);
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
			break;
		case BoundaryCollisionType::SOLID:
			//TODO : Make use of the radius and bounce on the interior of the boundaries
			if (p.x < 0.0f)		{	p.x = 0.0f;		p.vx *= -1.0f; }
			if (p.y < 0.0f)		{	p.y = 0.0f;		p.vy *= -1.0f; }
			if (p.x > m_width)	{	p.x = m_width;	p.vx *= -1.0f; }
			if (p.y > m_height) {	p.y = m_height; p.vy *= -1.0f; }
			break;
		case BoundaryCollisionType::WRAP:
			//TODO : Make use of the radius and wrap when the particle is out of the boundaries
			if (p.x < 0.0f)		{ p.x = m_width;  }
			if (p.y < 0.0f)		{ p.y = m_height; }
			if (p.x > m_width)	{ p.x = 0.0f;	  }
			if (p.y > m_height) { p.y = 0.0f;	  }
			break;
		}
	}
}

void Environment::draw(sf::RenderWindow * window)
{
	sf::CircleShape shape(static_cast<float>(RADIUS), 11);
	shape.setOrigin(static_cast<float>(RADIUS) / 2.0f, static_cast<float>(RADIUS) / 2.0f);
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
	//m_particles.resize(particleCount);
	
	int maxColorIndex = static_cast<int>(m_types.size()) - 1;
	assert(maxColorIndex > -1); // If you trigger the assertion evaluation make sur you created types before creating particles
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
}
