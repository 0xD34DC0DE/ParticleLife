#pragma once
#include <SFML/Graphics.hpp>
#include "GUIEventHandler.h"

enum EventType
{
	MOUSE_CLICK			= 1 << 0,
	KEYBOARD_KEYPRESS	= 1 << 1,
	MOUSE_MOVE			= 1 << 2,
	WINDOW_RESIZE		= 1 << 3,
};

template <EventType eventType>
class GUI
{
public:
	virtual bool update(sf::Event, CustomData* = nullptr) = 0; // CustomData is a nullptr so we can check its existance
	virtual void draw(sf::RenderTarget* renderTarget) {}; // default implementation needs to be in the header file since this is a template class
	void registerToHandler(GUIEventHandler& handler);
};

