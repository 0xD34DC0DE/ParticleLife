#include "GUI.h"

GUI::GUI()
{
}

void GUI::registerToHandler(GUIEventHandler & handler, EventType eventType)
{
	switch (eventType)
	{
	case EventType::KEYBOARD_KEYPRESS:
		handler.registerKeyboardKeyPressCallback(std::bind(&GUI::update, this, std::placeholders::_1));
		break;
	case EventType::MOUSE_CLICK:
		handler.registerMouseClickCallback(std::bind(&GUI::update, this, std::placeholders::_1));
		break;
	}
}