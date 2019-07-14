#pragma once
#include <SFML/Graphics.hpp>
#include "GUIEventHandler.h"

enum EventType
{
	MOUSE_CLICK			= 0,
	KEYBOARD_KEYPRESS	= 1,
};

class GUI
{
public:
	GUI();

	virtual bool update(sf::Event) = 0;
	void registerToHandler(GUIEventHandler& handler, EventType eventType);
};

