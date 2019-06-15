#include <SFML/Graphics.hpp>

#include "Environment.h"

// Work based on: https://github.com/HackerPoet/Particle-Life/blob/master/Universe.cpp

//TODO : Implement a batchDrawing class that renders the circle inside a texture atlas 
// and use a VertexBuffer to draw the shapes instead of calling ".draw()" for every shape

//TODO : implement more complex interactions: exemple: limit number of interactions per particle type, some interaction are stonger than average so they act like bonds
// and finally some non-linear attraction formula between certain types

int main()
{
	const unsigned int width = 500;
	const unsigned int height = 500;
	sf::RenderWindow window(sf::VideoMode(width, height), "Particle Life");
	window.setFramerateLimit(60);

	Environment env(width, height);
	env.setDebugDrawing(false);
	env.setDebugFlags(DebugDrawConfig::INTERACTION_LINE);
	env.setParams(-0.02f, 0.06f, 0.0f, 20.0f, 20.0f, 70.0f, 0.05f, false);

	env.addRandomTypes(7);

	env.setNeighboorSearchRadiusModeAuto(true);
	//env.setNeighboorSearchRadius(250.0f);

	env.createRandomParticles(250, 0.0f, 0.1f);
	env.setBoundaryCollisionType(WRAP);
	env.setDebugDrawingAlpha(1);

	const unsigned int updatePerFrame = 3;
	

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

		for(unsigned int i = 0; i < updatePerFrame; i++)
			env.update();

		env.draw(&window);

		window.display();
	}

	return 0;
}