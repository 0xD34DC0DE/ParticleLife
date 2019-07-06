#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

// Defaults: 60 FPS target, 1 sample, white font color, character size 10, fractionnal precision 2
class FPSCounter
{
public:
	
	FPSCounter(float targetFPS = 60.0f, unsigned int sampleCount = 1, std::string fontFileLocation = "");

	void setTextPos(float x, float y);
	void setCharacterSize(unsigned int size);
	void setFillColor(sf::Color fillColor);
	void setFontColor(sf::Color fontColor);
	void setFont(sf::Font font);
	void loadFontFromFile(std::string filePath);

	void setTargetFPS(float targetFPS);
	void setSampleCount(unsigned int sampleCount);

	void setFractionnalPrecision(unsigned int fractionnalNumberCount);

	void start();
	void stop();

	void draw(sf::RenderTarget* renderTarget);
private:

	float m_targetFPS;
	float m_targetMsPerFrame;
	unsigned int m_sampleCount;
	std::vector<float> m_FPSSamples;
	sf::Clock m_clock;
	unsigned int m_nextSampleIndex;

	sf::Rect<unsigned int> m_drawRect;
	float m_average;
	float m_sum;

	unsigned int m_fractionnalPrecision;

	sf::Font m_font;
	sf::Text m_text;


};

