#include "FPSCounter.h"


// Defaults: 60 FPS target, 1 sample, white font color, character size 10, fractionnal precision 2
FPSCounter::FPSCounter(float targetFPS, unsigned int sampleCount, std::string fontFileLocation)
{
	m_targetFPS = targetFPS;
	m_sampleCount = sampleCount;
	m_FPSSamples = std::vector<float>(sampleCount, 0.0f);
	m_average = 0.0f;
	m_sum = 0.0f;
	m_nextSampleIndex = 0;
	m_fractionnalPrecision = 2;
	
	m_font.loadFromFile(fontFileLocation);
	m_text.setFont(m_font);
	m_text.setPosition(0.0f, 0.0f);

	this->setTargetFPS(targetFPS);
}

void FPSCounter::setTextPos(float x, float y)
{
	m_text.setPosition(x, y);
}

void FPSCounter::setCharacterSize(unsigned int size)
{
	m_text.setCharacterSize(size);
}

void FPSCounter::setFillColor(sf::Color fillColor)
{
	m_text.setFillColor(fillColor);
}

void FPSCounter::setFontColor(sf::Color fontColor)
{
	m_text.setFillColor(fontColor);
}

void FPSCounter::setFont(sf::Font font)
{
	m_font = font;
	m_text.setFont(m_font);
}

void FPSCounter::loadFontFromFile(std::string filePath)
{
	m_font.loadFromFile(filePath);
	m_text.setFont(m_font);
}

void FPSCounter::setTargetFPS(float targetFPS)
{
	m_targetFPS = targetFPS;
	m_targetMsPerFrame = targetFPS / 1000.0f;
}

void FPSCounter::setSampleCount(unsigned int sampleCount)
{
	m_sampleCount = sampleCount;
	m_FPSSamples.resize(sampleCount, 0.0f);
}

void FPSCounter::setFractionnalPrecision(unsigned int fractionnalNumberCount)
{
	m_fractionnalPrecision = fractionnalNumberCount;
}

void FPSCounter::start()
{
	m_clock.restart();
}

void FPSCounter::stop()
{
	float milliseconds = m_clock.getElapsedTime().asMilliseconds();
	m_FPSSamples[m_nextSampleIndex] = milliseconds;

	// Reset index when reaching end of the sample vector
	if (++m_nextSampleIndex == m_sampleCount)
		m_nextSampleIndex = 0;

	// Moving average
	m_sum -= m_FPSSamples[m_nextSampleIndex];
	m_sum += milliseconds;

	m_average = m_sum / static_cast<float>(m_sampleCount);
}

void FPSCounter::draw(sf::RenderTarget * renderTarget)
{
	// Compute the ratio of the current average to the target (exemple: 0.5f ratio for 60 FPS yields 30 FPS)
	float targetRatio = (1.0f / m_targetMsPerFrame) / m_average;

	std::string fpsString = std::to_string(m_targetFPS * targetRatio);

	// find_first_of will find the position of the dot in the floating point number 
	// so we can have the same fractionnal precision even if the integer part changes in size
	m_text.setString(fpsString.substr(0, fpsString.find_first_of('.', 0) + m_fractionnalPrecision + 1));

	renderTarget->draw(m_text);
}
