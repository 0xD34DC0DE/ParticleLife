#include "Particle.h"

#include <algorithm> // for std::copy

#define RADIUS 5.0f

ParticleTypes::ParticleTypes()
{
	m_gen.seed(m_rd());
	m_typeCount = 0;
}

void ParticleTypes::resize(std::size_t size)
{
	m_color.resize(size);
}

void ParticleTypes::addRandomTypes(std::size_t typeCount)
{
	m_typeCount += typeCount; // Add total types count
	

	//TODO : setup parametisation for random values (normal distribution)
	std::uniform_real_distribution<float> randNormalized(0.0f, 1.0f);
	
	// If the new current type count is equal to the type count given as an argument,
	// we know that this is is the first call to this function and don't need to preserve already existing values
	if (m_typeCount == typeCount)
	{
		// Each type of particle has a unique set of interaction radius to other types so we allocate the square of the type count
		m_minRad.resize(m_typeCount * m_typeCount);
		m_maxRad.resize(m_typeCount * m_typeCount);
		m_attractForce.resize(m_typeCount * m_typeCount);

		//TODO : Pick up from here (Figuring out the correct way to generate the radiuses) https://github.com/HackerPoet/Particle-Life/blob/master/Universe.cpp
		for (int i = 0; i < m_typeCount; i++)
		{
			for (int j = 0; j < m_typeCount; j++)
			{
				if (i == j) 
				{
					m_attractForce[i * m_typeCount + j] = -randNormalized(m_gen) * 2.0f;
					m_minRad[i * m_typeCount + j] = RADIUS * 2.0f;
				}
				else
				{
					m_attractForce[i * m_typeCount + j] = randNormalized(m_gen) * 2.0f;
					m_minRad[i * m_typeCount + j] = std::max(randNormalized(m_gen) * 10.0f + 5.0f, RADIUS * 2.0f);
				}

				m_maxRad[i * m_typeCount + j] = std::max(randNormalized(m_gen) * 8.0f + 10.0f, m_minRad[i * m_typeCount + j]);

				// Radii symmetry
				m_maxRad[j * m_typeCount + i] = m_maxRad[i * m_typeCount + j];
				m_minRad[j * m_typeCount + i] = m_minRad[i * m_typeCount + j];
			}
		}
	}
	else
	{
		// Create the new vectors that will replace the old ones 
		std::vector<float> newMinRad;
		std::vector<float> newMaxRad;
		std::vector<float> newAttractForce;
		
		
		// (create bigger vector and copy from old instead of making a copy of the current one and enlarging it)
		newMinRad.resize(m_typeCount * m_typeCount);
		newMaxRad.resize(m_typeCount * m_typeCount);
		newAttractForce.resize(m_typeCount * m_typeCount);

		std::size_t oldCount = m_typeCount - typeCount;

		// Generate only new values (conserves previous)
		for (int i = 0; i < m_typeCount; i++)
		{
			if (i < oldCount)
			{
				for (int j = 0; j < typeCount; j++)
				{
					newAttractForce[i * m_typeCount + oldCount + j] = randNormalized(m_gen) * 2.0f;
					newMinRad[i * m_typeCount + oldCount + j] = std::max(randNormalized(m_gen) * 10.0f + 5.0f, RADIUS * 2.0f);

					newMaxRad[i * m_typeCount + oldCount + j] = std::max(randNormalized(m_gen) * 8.0f + 10.0f, newMinRad[i * m_typeCount + oldCount + j]);

					// Radii symmetry
					newMaxRad[j * m_typeCount + oldCount + i] = newMaxRad[i * m_typeCount + oldCount + j];
					newMinRad[j * m_typeCount + oldCount + i] = newMinRad[i * m_typeCount + oldCount + j];
				}

				std::copy(m_minRad.begin() + i * oldCount, m_minRad.begin() + (i + 1) * oldCount, newMinRad.begin() + i * m_typeCount);
				std::copy(m_maxRad.begin() + i * oldCount, m_maxRad.begin() + (i + 1) * oldCount, newMaxRad.begin() + i * m_typeCount);
				std::copy(m_attractForce.begin() + i * oldCount, m_attractForce.begin() + (i + 1) * oldCount, newAttractForce.begin() + i * m_typeCount);
			}
			else
			{
				for (int j = 0; j < m_typeCount; j++)
				{
					if (i == j)
					{
						newAttractForce[i * m_typeCount + j] = -randNormalized(m_gen) * 2.0f;
						newMinRad[i * m_typeCount + j] = RADIUS * 2.0f;
					}
					else
					{
						newAttractForce[i * m_typeCount + j] = randNormalized(m_gen) * 2.0f;
						newMinRad[i * m_typeCount + j] = std::max(randNormalized(m_gen) * 10.0f + 5.0f, RADIUS * 2.0f);
					}

					newMaxRad[i * m_typeCount + j] = std::max(randNormalized(m_gen) * 8.0f + 10.0f, newMinRad[i * m_typeCount + j]);

					// Radii symmetry
					newMaxRad[j * m_typeCount + i] = newMaxRad[i * m_typeCount + j];
					newMinRad[j * m_typeCount + i] = newMinRad[i * m_typeCount + j];
				}
			}
		}

		std::swap(m_minRad, newMinRad);
		std::swap(m_maxRad, newMaxRad);
		std::swap(m_attractForce, newAttractForce);
	}

	std::uniform_int_distribution<unsigned int> randColor(0, 255);
	for (unsigned int i = 0; i < typeCount; i++)
	{
		m_color.emplace_back(sf::Color(randColor(m_gen), randColor(m_gen), randColor(m_gen)));
	}
}

float ParticleTypes::minRadius(std::size_t type)
{
	return m_minRad[type * m_typeCount];
}

float ParticleTypes::maxRadius(std::size_t type)
{
	return m_maxRad[type * m_typeCount];
}

float ParticleTypes::attractForce(std::size_t type)
{
	return m_attractForce[type * m_typeCount];
}

