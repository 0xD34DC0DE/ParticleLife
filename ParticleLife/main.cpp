#include <SFML/Graphics.hpp>

#include "Environment.h"

// Work based on: https://github.com/HackerPoet/Particle-Life/blob/master/Universe.cpp

int main()
{
	const unsigned int width = 500;
	const unsigned int height = 500;
	sf::RenderWindow window(sf::VideoMode(width, height), "Particle Life");
	window.setFramerateLimit(60);

	Environment env(width, height);
	env.setDebugDrawing(true);
	env.setDebugFlags(DebugDrawConfig::INTERACTION_LINE | DebugDrawConfig::MAX_RADIUS | DebugDrawConfig::MIN_RADIUS);
	env.setParams(.0f, 0.06f, 0.0f, 20.0f, 10.0f, 50.0f, 0.1f, true);

	env.addRandomTypes(6);

	env.setNeighboorSearchRadiusModeAuto(true);
	//env.setNeighboorSearchRadius(250.0f);

	env.createRandomParticles(50, 0.0f, 0.1f);
	env.setBoundaryCollisionType(SOLID);
	env.setDebugDrawingAlpha(1);
	

	sf::Event evnt;
	while (window.isOpen())
	{
		while (window.pollEvent(evnt))
		{
			switch (evnt.type)
			{
			case sf::Event::Closed: // Window got closed
				window.close();
				break;
			default:
				break;
			}
		}

		window.clear();

		env.update();
		env.draw(&window);

		window.display();
	}

	return 0;
}