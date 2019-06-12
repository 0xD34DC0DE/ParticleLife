#include "Particle.h"

#include <algorithm> // for std::copy

#define RADIUS 5.0f
#define DIAMETER RADIUS * 2.0f

ParticleTypes::ParticleTypes()
{
	m_gen.seed(m_rd());
	m_typeCount = 0;
	m_friction = 0.0f;
	m_attract_mean = 0.0f;
	m_attract_std = 0.0f;
	m_minr_lower = 0.0f;
	m_minr_upper = 0.0f;
	m_maxr_lower = 0.0f;
	m_maxr_upper = 0.0f;
}

void ParticleTypes::setParams(float attract_mean, float attract_std, float minr_lower, float minr_upper, float maxr_lower, float maxr_upper, float friction)
{
	m_friction = friction;
	m_attract_mean = attract_mean;
	m_attract_std = attract_std;
	m_minr_lower = minr_lower;
	m_minr_upper = minr_upper;
	m_maxr_lower = maxr_lower;
	m_maxr_upper = maxr_upper;

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
	std::normal_distribution<float>       rand_attr(m_attract_mean, m_attract_std);
	std::uniform_real_distribution<float> rand_minr(m_minr_lower, m_minr_upper);
	std::uniform_real_distribution<float> rand_maxr(m_maxr_lower, m_maxr_upper);
	
	// If the new current type count is equal to the type count given as an argument,
	// we know that this is is the first call to this function and don't need to preserve already existing values
	if (m_typeCount == typeCount)
	{
		// Each type of particle has a unique set of interaction radius to other types so we allocate the square of the type count
		m_minRad.resize(m_typeCount * m_typeCount);
		m_maxRad.resize(m_typeCount * m_typeCount);
		m_attractForce.resize(m_typeCount * m_typeCount);

		//TODO : Pick up from here (Figuring out the correct way to generate the radiuses)
		for (int i = 0; i < m_typeCount; i++)
		{
			for (int j = 0; j < m_typeCount; j++)
			{
				if (i == j) 
				{
					m_attractForce[i * m_typeCount + j] = -std::abs(rand_attr(m_gen));
					m_minRad[i * m_typeCount + j] = DIAMETER;
				}
				else
				{
					m_attractForce[i * m_typeCount + j] = rand_attr(m_gen);
					m_minRad[i * m_typeCount + j] = std::max(rand_minr(m_gen), DIAMETER);
				}

				m_maxRad[i * m_typeCount + j] = std::max(rand_maxr(m_gen), m_minRad[i * m_typeCount + j]);

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
					newAttractForce[i * m_typeCount + oldCount + j] = rand_attr(m_gen);
					newMinRad[i * m_typeCount + oldCount + j] = std::max(rand_minr(m_gen), DIAMETER);

					newMaxRad[i * m_typeCount + oldCount + j] = std::max(rand_maxr(m_gen), newMinRad[i * m_typeCount + oldCount + j]);

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
						newAttractForce[i * m_typeCount + j] = -std::abs(rand_attr(m_gen));
						newMinRad[i * m_typeCount + j] = DIAMETER;
					}
					else
					{
						newAttractForce[i * m_typeCount + j] = rand_attr(m_gen);
						newMinRad[i * m_typeCount + j] = std::max(rand_minr(m_gen), DIAMETER);
					}

					newMaxRad[i * m_typeCount + j] = std::max(rand_maxr(m_gen), newMinRad[i * m_typeCount + j]);

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

float ParticleTypes::minRadius(std::size_t typeA, std::size_t typeB)
{
	return m_minRad[typeA * m_typeCount + typeB];
}

float ParticleTypes::maxRadius(std::size_t typeA, std::size_t typeB)
{
	return m_maxRad[typeA * m_typeCount + typeB];
}

float ParticleTypes::attractForce(std::size_t typeA, std::size_t typeB)
{
	return m_attractForce[typeA * m_typeCount + typeB];
}

unsigned int ParticleTypes::getTypeCount() const
{
	return m_typeCount;
}

