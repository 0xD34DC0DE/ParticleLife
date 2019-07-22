#pragma once
#include <SFML/Graphics.hpp>
#include "GUIEventHandler.h"

enum EventType
{
	MOUSE_CLICK,
	KEYBOARD_KEYPRESS,
	MOUSE_MOVE,
};

template <EventType eventType>
class GUI
{
public:
	virtual bool update(sf::Event, CustomData* = nullptr) = 0; // CustomData is a nullptr so we can check its existance
	virtual void draw(sf::RenderTarget* renderTarget) {}; // default implementation needs to be in the header file since this is a template class
	void registerToHandler(GUIEventHandler& handler);
};

