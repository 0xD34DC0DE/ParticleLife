#include "Environment.h"
#include <cassert>

#define RADIUS 5.0f
#define DIAMETER 2.0f * RADIUS
#define R_SMOOTH 2.0f


//TODO : store the min and max radius of an interaction for debug drawing

Environment::Environment(unsigned int width, unsigned int height)
{
	m_width = static_cast<float>(width);
	m_height = static_cast<float>(height);
	m_boundWidth = m_width;
	m_boundHeight = m_height;
	m_gen.seed(m_rd());
	m_bndColTy = BoundaryCollisionType::EMPTY;
	m_drawDebug = false;
	m_randByte = std::uniform_int_distribution<unsigned int>(0, 255);
	m_debugInteractionLines.setPrimitiveType(sf::PrimitiveType::Lines);
	m_debugDrawConf = DebugDrawConfig::NO_DEBUG_DRAW;
	m_particleCount = 0;
	m_useFlatForce = false;
	m_friction = 0.0f;
	m_attract_mean = 0.0f;
	m_attract_std = 0.0f;
	m_minr_lower = 0.0f;
	m_minr_upper = 0.0f;
	m_maxr_lower = 0.0f;
	m_maxr_upper = 0.0f;
	m_neighboorSearchRadius = std::sqrt(width * width + height * height);
	m_autoSearchNeighboorRadius = false;
}

sf::Color Environment::m_randomColor()
{
	return sf::Color(m_randByte(m_gen), m_randByte(m_gen), m_randByte(m_gen));
}


void Environment::update()
{
	const float r = static_cast<float>(RADIUS);

	if (m_drawDebug) // Only draw debug when its enabled
	{
		if (m_debugDrawConf & DebugDrawConfig::INTERACTION_LINE)
		{
			m_debugInteractionLines.clear();
		}
	}

	for (unsigned int i = 0; i < m_particleCount; i++)
	{
		Particle& p = m_particles[i];

		//TODO : Remove this simple gravity test to the actual genome based acceleration computation
		//TODO : Find a more stable Integrator
		unsigned int neighboorCount = m_getNeighbours(p, m_neighboorSearchRadius);

		const float fCoef = 1.0f;
		float accX = 0.0f;
		float accY = 0.0f;
		if (neighboorCount > 0)
			for (unsigned int j = 0; j < neighboorCount; j++)
			{
				const Particle& q = *m_neighboorPtrBuffer[j].first;

				if (m_drawDebug) // Only draw debug when its enabled
				{
					if (m_debugDrawConf & DebugDrawConfig::INTERACTION_LINE) // Collect the interaction lines
					{
						sf::Color interactionLineCol = m_types.color(p.type);
						interactionLineCol.a = 50;
						m_debugInteractionLines.append(sf::Vertex(sf::Vector2f(p.x, p.y), interactionLineCol));
						m_debugInteractionLines.append(sf::Vertex(sf::Vector2f(q.x, q.y), interactionLineCol));
					}
				}

				float dx = q.x - p.x;
				float dy = q.y - p.y;

				//Get distance squared
				const float r2 = m_neighboorPtrBuffer[j].second; // m_neighboorPtrBuffer[i].second -> Distance Squared
				const float minR = m_types.minRadius(p.type, q.type);
				const float maxR = m_types.maxRadius(p.type, q.type);

				if (r2 > maxR*maxR || r2 < 0.01f) {
					continue;
				}

				//Normalize displacement
				const float r = std::sqrt(r2);
				dx /= r;
				dy /= r;

				//Calculate force
				float f = 0.0f;
				if (r > minR) {
					if (m_useFlatForce) {
						f = m_types.attractForce(p.type, q.type);
					}
					else {
						const float numer = 2.0f * std::abs(r - 0.5f*(maxR + minR));
						const float denom = maxR - minR;
						f = m_types.attractForce(p.type, q.type) * (1.0f - numer / denom);
					}
				}
				else {
					f = R_SMOOTH * minR*(1.0f / (minR + R_SMOOTH) - 1.0f / (r + R_SMOOTH));
				}
				//Apply force
				p.vx += f * dx;
				p.vy += f * dy;
			}
	}

	for (unsigned int i = 0; i < m_particleCount; i++)
	{
		Particle& p = m_particles[i];

		p.x += p.vx;
		p.y += p.vy;
		p.vx *= 1.0f - m_friction;
		p.vy *= 1.0f - m_friction;

		switch (m_bndColTy)
		{
		case BoundaryCollisionType::EMPTY:
			// Boundaries are ignored
			break;
		case BoundaryCollisionType::SOLID:
			// Bounces of the walls
			if (p.x < r) { p.x = r;             p.vx *= -1.0f; }
			if (p.y < r) { p.y = r;             p.vy *= -1.0f; }
			if (p.x > m_boundWidth) { p.x = m_boundWidth;  p.vx *= -1.0f; }
			if (p.y > m_boundHeight) { p.y = m_boundHeight; p.vy *= -1.0f; }
			break;
		case BoundaryCollisionType::WRAP:
			// Wrap around the opposite edge
			if (p.x < -r) { p.x = m_boundWidth; }
			if (p.y < -r) { p.y = m_boundHeight; }
			if (p.x > m_boundWidth) { p.x = -r; }
			if (p.y > m_boundHeight) { p.y = -r; }
			break;
		}
	}

#ifdef DEBUG_UPDATE
	printf("total vx: %f.5\tvy: %f.5\n", total_vx, total_vy);
#endif // DEBUG_UPDATE
}

void Environment::draw(sf::RenderWindow * window)
{
	//TODO : Update drawing method to use SFML's Vertex buffer to draw the circles

	sf::CircleShape shape(static_cast<float>(RADIUS), 11);
	shape.setOrigin(static_cast<float>(RADIUS), static_cast<float>(RADIUS));
	shape.setOutlineColor(sf::Color::Transparent);
	shape.setOutlineThickness(1.0f);

	for (unsigned int i = 0; i < m_particleCount; i++)
	{
		Particle& p = m_particles[i];
		shape.setPosition(p.x, p.y);
		shape.setFillColor(m_types.color(p.type));
		shape.setOrigin(static_cast<float>(RADIUS), static_cast<float>(RADIUS));
		window->draw(shape);

		if (m_debugDrawConf != DebugDrawConfig::NO_DEBUG_DRAW)
		{
			m_debugDraw(window);
		}
	}
}

void Environment::m_debugDraw(sf::RenderWindow * window)
{
	sf::CircleShape shape(static_cast<float>(RADIUS), 11);
	shape.setOrigin(static_cast<float>(RADIUS), static_cast<float>(RADIUS));
	shape.setOutlineColor(sf::Color::Transparent);
	shape.setOutlineThickness(1.0f);

	for (unsigned int i = 0; i < m_particleCount; i++)
	{
		Particle& p = m_particles[i];
		shape.setPosition(p.x, p.y);
		shape.setFillColor(m_types.color(p.type));
		shape.setOrigin(static_cast<float>(RADIUS), static_cast<float>(RADIUS));
		window->draw(shape);

		if (m_drawDebug) // Only draw debug when its enabled
		{
			if (m_debugDrawConf & DebugDrawConfig::MIN_RADIUS)
			{
				shape.setFillColor(sf::Color::Transparent);
				// Min radius drawing
				shape.setOutlineColor(sf::Color::Yellow);
				float radius = m_types.minRadius(p.type, 0);
				shape.setRadius(radius);
				shape.setOrigin(radius, radius);
				window->draw(shape);
				shape.setOutlineColor(sf::Color::Transparent);
				shape.setRadius(static_cast<float>(RADIUS));
			}
		}
	}
	if (m_drawDebug) // Only draw debug when its enabled
	{
		if (m_debugDrawConf & DebugDrawConfig::INTERACTION_LINE)
		{
			window->draw(m_debugInteractionLines);
		}
	}
}

void Environment::createRandomParticles(std::size_t particleCount, float velMean, float velStd)
{
	m_particleCount = particleCount;

	m_neighboorPtrBuffer.resize(particleCount, std::make_pair(nullptr, NAN)); // Resize the pointer buffer used for the nearest neighbour query

	int maxColorIndex = static_cast<int>(m_types.size()) - 1;

#ifdef DEBUG
	assert(maxColorIndex > -1); // If you trigger the assertion evaluation make sure you created types before creating particles
#endif // DEBUG

	std::uniform_int_distribution<unsigned int> randCol(0, static_cast<unsigned int>(maxColorIndex));

	std::uniform_real_distribution<float> randUniPosW(0.0f, static_cast<float>(m_width));
	std::uniform_real_distribution<float> randUniPosH(0.0f, static_cast<float>(m_height));

	std::normal_distribution<float> randNormVel(velMean, velStd);

	for (unsigned i = 0; i < particleCount; i++)
	{
		m_particles.emplace_back(randUniPosW(m_gen), randUniPosH(m_gen), randNormVel(m_gen), randNormVel(m_gen), (uint8_t)(randCol(m_gen)));
	}
}

void Environment::addRandomTypes(std::size_t typeCount)
{
	m_types.addRandomTypes(typeCount);

	if (m_autoSearchNeighboorRadius)
	{
		// Searches trough every max radius to get the biggest one and sets this as the new limit when searching for interactions
		float maxR = 0.0f;
		for (int i = 0; i < m_types.getTypeCount(); i++)
		{
			for (int j = 0; j < m_types.getTypeCount(); j++)
			{
				if (maxR < m_types.maxRadius(i, j))
				{
					maxR = m_types.maxRadius(i, j);
				}
			}
		}
		// if maxR is still 0, defaults to the length of the diagonal of the environment
		m_neighboorSearchRadius = (maxR != 0.0f) ? maxR : std::sqrt(m_width * m_width + m_height * m_height);
	}
}

void Environment::setBoundaryCollisionType(BoundaryCollisionType bndColTy)
{
	m_bndColTy = bndColTy;

	switch (bndColTy)
	{
	case BoundaryCollisionType::EMPTY:
		// Bounds are the same size as the environment size since we ignore them anyway
		m_boundWidth = m_width;
		m_boundHeight = m_height;
		break;
	case BoundaryCollisionType::SOLID:
		// Bounds are the environment size minus the balls radius to make sure the balls bounce while
		// they are still onscreen
		m_boundWidth = m_width - static_cast<float>(RADIUS);
		m_boundHeight = m_height - static_cast<float>(RADIUS);
		break;
	case BoundaryCollisionType::WRAP:
		// Bounds are the environment size plus the balls radius to make sure the wrapping happens
		// when the balls are offscreen
		m_boundWidth = m_width + static_cast<float>(RADIUS);
		m_boundHeight = m_height + static_cast<float>(RADIUS);
		break;
	}
}

void Environment::setDebugDrawing(bool enabled)
{
	m_debugDrawConf &= static_cast<unsigned int>(enabled);
}

void Environment::setDebugFlags(unsigned int flags)
{
	m_debugDrawConf = flags;
}

void Environment::setDebugFlags(unsigned int flag, bool value)
{
	(value) ? m_debugDrawConf |= flag : m_debugDrawConf &= ~flag;
}

void Environment::toggleDebugFlag(unsigned int flag)
{
	m_debugDrawConf = m_debugDrawConf ^ flag;
}

void Environment::setNeighboorSearchRadius(float radius)
{
	m_neighboorSearchRadius = radius;
}

void Environment::setNeighboorSearchRadiusModeAuto(bool enabled)
{
	m_autoSearchNeighboorRadius = enabled;
	if (enabled)
	{
		// Searches trough every max radius to get the biggest one and sets this as the new limit when searching for interactions
		float maxR = 0.0f;
		for (int i = 0; i < m_types.getTypeCount(); i++)
		{
			for (int j = 0; j < m_types.getTypeCount(); j++)
			{
				if (maxR < m_types.maxRadius(i, j))
				{
					maxR = m_types.maxRadius(i, j);
				}
			}
		}
		// if maxR is still 0, defaults to the length of the diagonal of the environment
		m_neighboorSearchRadius = (maxR != 0.0f) ? maxR : std::sqrt(m_width * m_width + m_height * m_height);
	}
}

void Environment::setParams(float attract_mean, float attract_std, float minr_lower, float minr_upper, float maxr_lower, float maxr_upper, float friction, bool flat_force)
{
	m_friction = friction;
	m_attract_mean = attract_mean;
	m_attract_std = attract_std;
	m_minr_lower = minr_lower;
	m_minr_upper = minr_upper;
	m_maxr_lower = maxr_lower;
	m_maxr_upper = maxr_upper;
	m_useFlatForce = flat_force;
	m_types.setParams(attract_mean, attract_std, minr_lower, minr_upper, maxr_lower, maxr_upper, friction); // Pass the arguments to the particle type class
}

unsigned int Environment::m_getNeighbours(const Particle& particle, float searchRadius)
{
	//TODO : Replace with a quad-tree or another space partitioning data structure

	unsigned int neighboorCount = 0; // serves as index to the next slot in the pointer buffer if we found a neighboor and also as return value
	const float origin_x = particle.x;
	const float origin_y = particle.y;
	const float radSqr = searchRadius * searchRadius;

	for (unsigned int i = 0; i < m_particleCount; i++)
	{
		const Particle& p = m_particles[i]; // doesn't need to be const but it is to avoid accidental modification to the value

		if (&p == &particle) // Skip itself
			continue;

		float dx = origin_x - p.x;
		float dy = origin_y - p.y;
		float distSqr = (dx * dx) + (dy * dy);
		if (distSqr < radSqr)
		{
			m_neighboorPtrBuffer[neighboorCount].first = const_cast<Particle*>(&p); // Add particle to the list of found neighboors
			m_neighboorPtrBuffer[neighboorCount].second = distSqr; // Store the squared distance
			neighboorCount++;
		}
	}

	return neighboorCount;
}
