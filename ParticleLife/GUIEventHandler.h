#pragma once
#include <functional>
#include <vector>
#include <tuple>
#include <SFML/Graphics.hpp>

struct CustomData
{
	CustomData() : size(0U), ptr(nullptr) {};
	CustomData(std::size_t dataSize, void* dataPtr) : size(dataSize), ptr(dataPtr) {};
	std::size_t size;
	void* ptr;
};

typedef std::function<bool(sf::Event, CustomData*)> sfmlEventCallback;

// GUIEventHandler stores events in separate vectors depending on the type of the event called
// this is so we can make a virtual function that accepts sfml's event to be flexible but registers only to the type of events it needs
// so we don't send a mouseEvent to a function that expect a keyEvent since both of the events are of type sf::Event
//TODO : Find better way to do that
class GUIEventHandler
{
public:
	GUIEventHandler();
	~GUIEventHandler();
	
	void registerKeyboardKeyPressCallback(sfmlEventCallback callback);
	void registerMouseClickCallback(sfmlEventCallback callback);
	void registerMouseMoveCallback(sfmlEventCallback callback);
	void registerWindowResizeCallback(sfmlEventCallback callback);

	// the data will be copied into a customData struct to prevent dangling pointers
	void addEvent(sf::Event sfmlEvent, CustomData* data = nullptr);

	//return number of processed events
	unsigned int processEvents();
private:

	CustomData m_createCustomData(std::size_t size, const void* data);
	void m_clearCustomData();

	std::vector<sf::Event> m_keyEvents;
	std::vector<sf::Event> m_mouseClickEvents;
	std::vector<sf::Event> m_mouseMoveEvents;
	std::vector<sf::Event> m_windowResizeEvents;

	std::vector<sfmlEventCallback> m_registeredKeyboardFunctions;
	std::vector<sfmlEventCallback> m_registeredMouseClickFunctions;
	std::vector<sfmlEventCallback> m_registeredMouseMoveFunctions;
	std::vector<sfmlEventCallback> m_registeredWindowResizeFunctions;

	unsigned int m_activeCustomData;
	unsigned int m_customDataCount;
	std::vector<CustomData> m_customDatas;
};

