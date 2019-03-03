#include "Particle.h"

ParticleTypes::ParticleTypes()
{
	m_gen.seed(m_rd());
}

void ParticleTypes::resize(std::size_t size)
{
	m_color.resize(size);
}

void ParticleTypes::addRandomTypes(std::size_t typeCount)
{
	m_typeCount = typeCount;
	std::uniform_int_distribution<unsigned int> randColor(0, 255);
	for (unsigned int i = 0; i < typeCount; i++)
	{
		m_color.emplace_back(sf::Color(randColor(m_gen), randColor(m_gen), randColor(m_gen)));
	}
}

