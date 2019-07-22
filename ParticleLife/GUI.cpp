#include "GUI.h"

template<>
void GUI<EventType::KEYBOARD_KEYPRESS>::registerToHandler(GUIEventHandler& handler)
{
	handler.registerKeyboardKeyPressCallback(std::bind(&GUI::update, this, std::placeholders::_1, std::placeholders::_2));
}

template<>
void GUI<EventType::MOUSE_CLICK>::registerToHandler(GUIEventHandler& handler)
{
	handler.registerMouseClickCallback(std::bind(&GUI::update, this, std::placeholders::_1, std::placeholders::_2));
}

template<>
void GUI<static_cast<EventType>(EventType::MOUSE_CLICK | EventType::MOUSE_MOVE)>::registerToHandler(GUIEventHandler& handler)
{
	handler.registerMouseClickCallback(std::bind(&GUI::update, this, std::placeholders::_1, std::placeholders::_2));
	handler.registerMouseMoveCallback(std::bind(&GUI::update, this, std::placeholders::_1, std::placeholders::_2));
}