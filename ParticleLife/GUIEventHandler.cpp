#include "GUIEventHandler.h"



GUIEventHandler::GUIEventHandler()
{
}

void GUIEventHandler::registerKeyboardKeyPressCallback(sfmlEventCallback callback)
{
	m_registeredKeyboardFunctions.emplace_back(callback);
}

void GUIEventHandler::registerMouseClickCallback(sfmlEventCallback callback)
{
	m_registeredMouseFunctions.emplace_back(callback);
}

void GUIEventHandler::addEvent(sf::Event sfmlEvent)
{
	switch (sfmlEvent.type)
	{
	case sf::Event::EventType::KeyPressed:
	case sf::Event::EventType::KeyReleased:
		m_keyEvents.emplace_back(sfmlEvent);
		break;
		//TODO : implement a custom event for "complete" mouse clicks (press + release) (maybe just release is the right approach)
	case sf::Event::EventType::MouseButtonPressed:
	case sf::Event::EventType::MouseButtonReleased:
		m_mouseEvents.emplace_back(sfmlEvent);
		break;
	}
	
}

unsigned int GUIEventHandler::processEvents()
{
	unsigned int processedEventsCount = 0;

	for (auto& evnt : m_keyEvents)
		for (auto& cb : m_registeredKeyboardFunctions)
			if(cb(evnt))
				processedEventsCount++;

	for (auto& evnt : m_mouseEvents)
		for (auto& cb : m_registeredMouseFunctions)
			if (cb(evnt))
				processedEventsCount++;

	m_keyEvents.clear();
	m_mouseEvents.clear();

	return processedEventsCount;
}
