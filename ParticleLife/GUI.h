#pragma once
#include <SFML/Graphics.hpp>
#include "GUIEventHandler.h"

class GUI
{
public:
	GUI();

	virtual void update(sf::Event) = 0;
	virtual void registerToHandler(GUIEventHandler& handler) = 0;
};

