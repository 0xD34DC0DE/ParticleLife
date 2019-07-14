#pragma once
#include <functional>
#include <vector>
#include <SFML/Graphics.hpp>

typedef std::function<bool(sf::Event)> sfmlEventCallback;

// GUIEventHandler stores events in separate vectors depending on the type of the event called
// this is so we can make a virtual function that accepts sfml's event to be flexible but registers only to the type of events it needs
// so we don't send a mouseEvent to a function that expect a keyEvent since both of the events are of type sf::Event

class GUIEventHandler
{
public:
	GUIEventHandler();
	
	void registerKeyboardKeyPressCallback(sfmlEventCallback callback);
	void registerMouseClickCallback(sfmlEventCallback callback);

	void addEvent(sf::Event sfmlEvent);

	//return number of processed events
	unsigned int processEvents();
private:

	std::vector<sf::Event> m_keyEvents;
	std::vector<sf::Event> m_mouseEvents;

	std::vector<sfmlEventCallback> m_registeredKeyboardFunctions;
	std::vector<sfmlEventCallback> m_registeredMouseFunctions;

};

