#include <SFML/Graphics.hpp>

#include "Environment.h"
#include "FPSCounter.h"
#include "GUIParticleSelector.h"
#include "KeyboardMapper.h"

// Work based on: https://github.com/HackerPoet/Particle-Life/blob/master/Universe.cpp

//TODO : implement more complex interactions: example: limit the number of interactions per particle type, some interaction are stonger than average so they act like bonds
// and finally some non-linear attraction formula between certain types

//TODO : Investigate stability when density of particles in same location is high

//TODO : Add a way to select a particle type and randomize its values (needs to propagate to other types too)

//TODO : Add boundary type that repulse particle so they don't stick to the walls and bunch up in corners

//TODO : Make TEXTURE_RES arguments use a resolution relative to the resolution  of the window

//TODO : Make a registering function for the GUIHandler class for automatic drawing

//TODO : Allow non squarred matrix attaraction forces(lets certain particle not attract/repusle each other but they still collide)

//TODO : Add size and mass
int main()
{
	const unsigned int width = 720;
	const unsigned int height = 480;
	sf::RenderWindow window(sf::VideoMode(width, height), "Particle Life");
	window.setFramerateLimit(60);

	Environment env(width, height);
	env.setDebugDrawing(true);
	env.setDebugFlags(DebugDrawConfig::INTERACTION_LINE);
	env.setDebugDrawInteractionLineStyle(InteractionLineStyle::SRC_DEST_MIX);
	env.setParams(-0.02f, 0.06f, 0.0f, 20.0f, 20.0f, 70.0f, 0.05f, false);

	env.addRandomTypes(7);

	env.setNeighboorSearchRadiusModeAuto(true);
	//env.setNeighboorSearchRadius(250.0f);

	env.createRandomParticles(256, 0.0f, 0.1f);
	env.setBoundaryCollisionType(REPUSLE);
	env.setDebugDrawingAlpha(20);

	const unsigned int updatePerFrame = 2;

	FPSCounter fpsCounter(60.0f, 15, "./../ressources/ARIAL.ttf");

	unsigned int tick = 0;

	GUIEventHandler guiHandler;
	
	GUIParticleSelector guiParticleSelector(env.getSize(), window.getSize(), env.getParticleRadius(), 5.0f, 15.0f, 32, env.getParticleVectorPtr());
	guiParticleSelector.registerToHandler(guiHandler);

	KeyboardMapper keyMapper;
	keyMapper.registerToHandler(guiHandler);
	keyMapper.mapKeyToFunction(sf::Keyboard::R, sf::Event::KeyReleased, std::bind(&Environment::reset, &env));

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
			guiHandler.addEvent(evnt);
		}

		int processedEventCount = guiHandler.processEvents();
		if (processedEventCount != 0)
			printf("Events processed at frame %i: %i\n", tick, processedEventCount);

		fpsCounter.start();

		window.clear();
		
		env.update(updatePerFrame);

		env.draw(&window);

		guiParticleSelector.draw(&window);
		
		fpsCounter.draw(&window);

		window.display();

		fpsCounter.stop();

		tick++;
	}

	return 0;
}