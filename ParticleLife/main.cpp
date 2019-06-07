#include <SFML/Graphics.hpp>

#include "Environment.h"

int main()
{
	const unsigned int width = 500;
	const unsigned int height = 500;
	sf::RenderWindow window(sf::VideoMode(width, height), "Particle Life");
	window.setFramerateLimit(60);

	Environment env(width, height);
	env.setDebugDrawing(true);
	env.setDebugFlags(DebugDrawConfig::INTERACTION_LINE);
	env.setParams(0.02f, 0.04f, 0.0f, 30.0f, 30.0f, 100.0f, 0.01f, false);

	// Testing old value preservation by calling with 1 and 2 instead of a single time with 3
	env.addRandomTypes(9);

	env.setNeighboorSearchRadiusModeAuto(true);

	env.createRandomParticles(150, 0.0f, 0.1f);
	env.setBoundaryCollisionType(SOLID);
	

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