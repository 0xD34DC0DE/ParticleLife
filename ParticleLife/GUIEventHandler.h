#pragma once
#include <functional>
#include <vector>
#include <SFML/Graphics.hpp>

//TODO : investigate how to make that work
/*
typedef std::function<void(sf::Event::MouseButtonEvent)> MouseClickFunction;
typedef std::function<void(sf::Event::KeyEvent)> KeyboardKeyPressFunction;
*/
typedef std::function<std::function<void(sf::Event)>> MouseClickFunction;
typedef std::function<std::function<void(sf::Event)>> KeyboardKeyPressFunction;

class GUIEventHandler
{
public:
	GUIEventHandler();

	void registerMouseClickEventFunction(std::function<void(sf::Event)> mouseFunc);
	void registerKeyboardKeyPressEventFunction(std::function<void(sf::Event)> keyboardFunc);

	void addEvent(sf::Event sfmlEvent);

	//return number of processed events
	unsigned int processEvents();
private:
	std::vector<std::function<void(sf::Event)>> m_mouseClickFunctions;
	std::vector<std::function<void(sf::Event)>> m_keyboardKeyPressFunctions;

	std::vector<sf::Event> m_mouseClickEvents;
	std::vector<sf::Event> m_keyboardKeyPressEvents;
};

