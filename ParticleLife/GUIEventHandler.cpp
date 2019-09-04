#include "GUIEventHandler.h"

GUIEventHandler::GUIEventHandler()
{
	m_activeCustomData = 0;
}

GUIEventHandler::~GUIEventHandler()
{
	// Free all allocated memory for custom data
	for (auto& customData : m_customDatas)
		if (customData.size > 0)
			std::free(customData.ptr);
}

void GUIEventHandler::registerKeyboardKeyPressCallback(sfmlEventCallback callback)
{
	m_registeredKeyboardFunctions.emplace_back(callback);
}

void GUIEventHandler::registerMouseClickCallback(sfmlEventCallback callback)
{
	m_registeredMouseClickFunctions.emplace_back(callback);
}

void GUIEventHandler::registerMouseMoveCallback(sfmlEventCallback callback)
{
	m_registeredMouseMoveFunctions.emplace_back(callback);
}

void GUIEventHandler::registerWindowResizeCallback(sfmlEventCallback callback)
{
	m_registeredWindowResizeFunctions.emplace_back(callback);
}

void GUIEventHandler::addEvent(sf::Event sfmlEvent, CustomData* data)
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
		m_mouseClickEvents.emplace_back(sfmlEvent);
		break;
	case sf::Event::EventType::MouseMoved:
			m_mouseMoveEvents.emplace_back(sfmlEvent);
		break;
	case sf::Event::Resized:
		m_windowResizeEvents.emplace_back(sfmlEvent);
		break;
	}
	
}

unsigned int GUIEventHandler::processEvents()
{
	unsigned int processedEventsCount = 0;

	for (auto& evnt : m_keyEvents)
		for (auto& cb : m_registeredKeyboardFunctions)
			if(cb(evnt, nullptr))
				processedEventsCount++;

	for (auto& evnt : m_mouseClickEvents)
		for (auto& cb : m_registeredMouseClickFunctions)
			if (cb(evnt, nullptr))
				processedEventsCount++;

	for (auto& evnt : m_mouseMoveEvents)
		for (auto& cb : m_registeredMouseMoveFunctions)
			if (cb(evnt, nullptr))
				processedEventsCount++;

	for (auto& evnt : m_windowResizeEvents)
		for (auto& cb : m_registeredWindowResizeFunctions)
			if (cb(evnt, nullptr))
				processedEventsCount++;

	m_keyEvents.clear();
	m_mouseClickEvents.clear();
	m_mouseMoveEvents.clear();
	m_windowResizeEvents.clear();

	m_clearCustomData();
	
	return processedEventsCount;
}

CustomData GUIEventHandler::m_createCustomData(std::size_t size, const void * data)
{
	m_activeCustomData++;
	//Create a new customData struct in the vector if no more free customData struct are available
	if (m_customDataCount < m_activeCustomData)
	{
		m_customDatas.emplace_back(size, std::malloc(size));
		m_customDataCount++;
	}

	CustomData& customData = m_customDatas[m_activeCustomData - 1]; // Get a reference to the next available customData struct

	if (customData.size < size)
		std::realloc(customData.ptr, size);
	customData.size = size;

	std::memcpy(customData.ptr, data, size); // we copy the data provided by the data pointer because we don't know its lifetime (avoids dangling pointer)

	return customData; // Return active "customData"
}

void GUIEventHandler::m_clearCustomData()
{
	m_activeCustomData = 0;
}
