#include "GUI.h"

//template<EventType eventType>
//void GUI<eventType>::draw(sf::RenderTarget* renderTarget)
//{}

template<>
void GUI<EventType::KEYBOARD_KEYPRESS>::registerToHandler(GUIEventHandler& handler)
{
	handler.registerKeyboardKeyPressCallback(std::bind(&GUI::update, this, std::placeholders::_1));
}

template<>
void GUI<EventType::MOUSE_CLICK>::registerToHandler(GUIEventHandler& handler)
{
	handler.registerMouseClickCallback(std::bind(&GUI::update, this, std::placeholders::_1));
}