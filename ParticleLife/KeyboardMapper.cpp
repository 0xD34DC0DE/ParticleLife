#include "KeyboardMapper.h"

KeyboardMapper::KeyboardMapper()
{
}

bool KeyboardMapper::update(sf::Event sfmlEvent, CustomData*)
{
	auto it = m_keyToFunctionMap.find(std::make_pair(sfmlEvent.key.code, sfmlEvent.type));
	if (it != m_keyToFunctionMap.end())
	{
		it->second(); // Call the function associated with this key
		return true;
	}
	return false;
}

void KeyboardMapper::mapKeyToFunction(sf::Keyboard::Key key, sf::Event::EventType keyEventType, std::function<void(void)> function)
{
	m_keyToFunctionMap.emplace(std::make_pair(key, keyEventType), function);
}
