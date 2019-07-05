#include <SFML/Graphics.hpp>

#include "Environment.h"

#include "BatchRenderer2D.h"

// Work based on: https://github.com/HackerPoet/Particle-Life/blob/master/Universe.cpp

//TODO : Implement a batchDrawing class that renders the circle inside a texture atlas 
// and use a VertexBuffer to draw the shapes instead of calling ".draw()" for every shape

//TODO : implement more complex interactions: example: limit the number of interactions per particle type, some interaction are stonger than average so they act like bonds
// and finally some non-linear attraction formula between certain types

//TODO : Check if the FPS computation is accurate (debug draw-> 100fps+, no debugDraw -> 60fps?)

int main()
{
	const unsigned int width = 1024;
	const unsigned int height = 1024;
	sf::RenderWindow window(sf::VideoMode(width, height), "Particle Life");
	window.setFramerateLimit(60);

	Environment env(width, height);
	env.setDebugDrawing(false);
	env.setDebugFlags(DebugDrawConfig::INTERACTION_LINE);
	env.setParams(-0.02f, 0.06f, 0.0f, 20.0f, 20.0f, 70.0f, 0.05f, false);

	env.addRandomTypes(7);

	env.setNeighboorSearchRadiusModeAuto(true);
	//env.setNeighboorSearchRadius(250.0f);

	env.createRandomParticles(256, 0.0f, 0.1f);
	env.setBoundaryCollisionType(SOLID);
	env.setDebugDrawingAlpha(1);

	const unsigned int updatePerFrame = 2;

	// FPS Counter stuff
	sf::Clock clk;
	float times[5]{0.0f};
	unsigned int tick = 0;
	sf::Font FPSFont;
	FPSFont.loadFromFile("./../ressources/ARIAL.ttf");
	sf::Text FPSText("", FPSFont);
	FPSText.setFillColor(sf::Color::White);
	FPSText.setCharacterSize(20);

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

		clk.restart();
		window.clear();
		
		for(unsigned int i = 0; i < updatePerFrame; i++)
			env.update();

		env.draw(&window);

		window.draw(FPSText);

		window.display();


		times[tick % 5] = 60.0f * (60.0f / 1000.0f) * clk.getElapsedTime().asMilliseconds();
		float avg = 0.0f;
		for (unsigned int i = 0; i < 5; i++)
			avg += times[i];
		avg /= 5.0f;

		FPSText.setString(std::to_string(avg).substr(0, 5));

		tick++;
	}

	return 0;
}