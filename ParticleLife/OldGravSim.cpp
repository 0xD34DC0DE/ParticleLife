#if GRAV_SIM
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
#endif //GRAV_SIM