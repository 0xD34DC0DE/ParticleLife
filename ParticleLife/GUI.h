#pragma once
#include <SFML/Graphics.hpp>
#include "GUIEventHandler.h"

enum EventType
{
	MOUSE_CLICK,
	KEYBOARD_KEYPRESS,
};

template <EventType eventType>
class GUI
{
public:
	virtual bool update(sf::Event) = 0;
	virtual void draw(sf::RenderTarget* renderTarget) {}; // default implementation needs to be in the header file since this is a template class
	void registerToHandler(GUIEventHandler& handler);
};

