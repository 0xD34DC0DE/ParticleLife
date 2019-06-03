#include "Environment.h"
#include <cassert>

#define RADIUS 5
#define DELTA_T 0.5f
#define DELTA_T_FRAME 0.5f

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
}

sf::Color Environment::m_randomColor()
{
	return sf::Color(m_randByte(m_gen), m_randByte(m_gen), m_randByte(m_gen));
}

void Environment::update()
{
	const float r = static_cast<float>(RADIUS);

	bool debugLastIteration = false;
	if (m_debugDrawConf & DebugDrawConfig::INTERACTION_LINE)
	{
		m_debugInteractionLines.clear();
	}

	float total_vx = 0.0f, total_vy = 0.0f;
	
	float t = 0.0f;
	while (t < DELTA_T_FRAME)
	{
		// True at the last iteration
		debugLastIteration = (t + DELTA_T > DELTA_T_FRAME - DELTA_T);
		

		for (unsigned int i = 0; i < m_particleCount; i++)
		{
			Particle& p = m_particles[i];

			//TODO : Remove this simple gravity test to the actual genome based acceleration computation
			//TODO : Find a more stable Integrator
			unsigned int neighboorCount = m_getNeighbours(p, 100.0f);

			const float fCoef = 1.0f;
			float accX = 0.0f;
			float accY = 0.0f;
			if (neighboorCount > 0)
				for (unsigned int i = 0; i < neighboorCount; i++)
				{
					const Particle& q = *m_neighboorPtrBuffer[i].first;
					
					if (m_debugDrawConf & DebugDrawConfig::INTERACTION_LINE)
					{
						// Store infos for debug drawing
						if (debugLastIteration)
						{
							m_debugInteractionLines.append(sf::Vertex(sf::Vector2f(q.x, q.y), m_types.color(p.type)));
							m_debugInteractionLines.append(sf::Vertex(sf::Vector2f(p.x, p.y), m_types.color(p.type)));
						}
					}

					float dx = q.x - p.x;
					float dy = q.y - p.y;
					float invMag = 1.0f / std::powf(m_neighboorPtrBuffer[i].second, 0.5f); // m_neighboorPtrBuffer[i].second -> Distance Squared
					float force = fCoef / std::powf(m_neighboorPtrBuffer[i].second, 0.5f);

					accX += force * (dx * invMag);
					accY += force * (dy * invMag);
				}
			p.vx += accX * DELTA_T;
			p.vy += accY * DELTA_T;
		}

		for (unsigned int i = 0; i < m_particleCount; i++)
		{
			Particle& p = m_particles[i];
		
			p.x += p.vx * DELTA_T;
			p.y += p.vy * DELTA_T;

			total_vx += p.vx;
			total_vy += p.vy;

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
		t += DELTA_T;
	}
#ifdef DEBUG_UPDATE
	printf("total vx: %f.5\tvy: %f.5\n", total_vx, total_vy);
#endif // DEBUG_UPDATE
}

void Environment::draw(sf::RenderWindow * window)
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

		if (m_debugDrawConf & DebugDrawConfig::MIN_RADIUS)
		{
			shape.setFillColor(sf::Color::Transparent);
			// Min radius drawing
			shape.setOutlineColor(sf::Color::Yellow);
			float radius = m_types.minRadius(p.type);
			shape.setRadius(radius);
			shape.setOrigin(radius, radius);
			window->draw(shape);
			shape.setOutlineColor(sf::Color::Transparent);
			shape.setRadius(static_cast<float>(RADIUS));
		}
	}

	if (m_debugDrawConf & DebugDrawConfig::INTERACTION_LINE)
	{
		window->draw(m_debugInteractionLines);
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
