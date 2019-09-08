#pragma once
#include "GUI.h"
#include <functional>
#include <map>
#include <tuple>

class KeyboardMapper : public GUI<EventType::KEYBOARD_KEYPRESS>
{
public:
	KeyboardMapper();
	virtual bool update(sf::Event, CustomData* = nullptr) override final;

	void mapKeyToFunction(sf::Keyboard::Key key, sf::Event::EventType keyEventType, std::function<void(void)> function);

private:
	std::map<std::pair<sf::Keyboard::Key, sf::Event::EventType>, std::function<void(void)>> m_keyToFunctionMap;
};

