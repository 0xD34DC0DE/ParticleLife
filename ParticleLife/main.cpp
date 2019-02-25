#include <SFML/Graphics.hpp>

#include "Environment.h"

int main()
{
	const unsigned int width = 512;
	const unsigned int height = 512;
	sf::RenderWindow window(sf::VideoMode(width, height), "Particle Life");
	window.setFramerateLimit(60);

	Environment env(width, height);
	env.createRandomTypes(3);
	env.createRandomParticles(10, 0.0f, 1.0f);
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