#include "GUIEventHandler.h"



GUIEventHandler::GUIEventHandler()
{
}

void GUIEventHandler::registerMouseClickEventFunction(std::function<void(sf::Event)> mouseFunc)
{
	m_mouseClickFunctions.emplace_back(mouseFunc);
}

void GUIEventHandler::registerKeyboardKeyPressEventFunction(std::function<void(sf::Event)> keyboardFunc)
{
	m_keyboardKeyPressFunctions.emplace_back(keyboardFunc);
}

void GUIEventHandler::addEvent(sf::Event sfmlEvent)
{
	switch (sfmlEvent.type)
	{
	case sf::Event::EventType::KeyPressed:
		m_keyboardKeyPressFunctions.emplace_back(sfmlEvent);
		break;
	case sf::Event::EventType::MouseMoved:
		m_mouseClickEvents.emplace_back(sfmlEvent);
		break;
	case sf::Event::EventType::MouseButtonPressed:
		m_mouseClickEvents.emplace_back(sfmlEvent);
		break;
	case sf::Event::EventType::MouseButtonReleased:
		m_mouseClickEvents.emplace_back(sfmlEvent);
		break;
	}
}

unsigned int GUIEventHandler::processEvents()
{
	unsigned int processedEventsCount;

	//TODO : make it so std::function are not void and use boolean(when they accept the event (exemple: it is within bounds of an registered GUI element))

	for (auto &keyboardEvent : m_keyboardKeyPressEvents)
		for (auto &keyboardCallback : m_keyboardKeyPressFunctions)
		{
			keyboardCallback(keyboardEvent);
			processedEventsCount++;
		}

	for (auto &mouseEvent : m_mouseClickEvents)
		for (auto &mouseCallback : m_mouseClickFunctions)
		{
			mouseCallback(mouseEvent);
			processedEventsCount++;
		}

	m_keyboardKeyPressEvents.clear();
	m_mouseClickEvents.clear();

	return processedEventsCount;
}
