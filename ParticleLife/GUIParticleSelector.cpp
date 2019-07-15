#include "GUIParticleSelector.h"


GUIParticleSelector::GUIParticleSelector(float radius, unsigned int textureRes, std::vector<Particle>* particleVectorPtr, sf::Color highlighColor)
{
	m_particleVectorPtr = particleVectorPtr;
	m_radius = radius;
	m_distSquared = static_cast<unsigned int>(radius * radius);
}

void GUIParticleSelector::draw(sf::RenderTarget* renderTarget)
{
	// Skips drawing if no particle is selected
	if (m_particleSelected)
	{
		//TODO : implement drawing
		m_particleSelected = false;
	}
}

bool GUIParticleSelector::update(sf::Event evnt)
{
	if (evnt.mouseButton.button == sf::Mouse::Button::Left && evnt.type == sf::Event::EventType::MouseButtonReleased)
	{
		Particle* closestParticle = m_getSelectedParticle(evnt.mouseButton.x, evnt.mouseButton.y); // will be nullptr if no particle is got selected
		if (closestParticle != nullptr)
		{
			m_particleSelected = true;
		}
		// doesn't need coordinate check since this encapsulate the whole window
		return true;
	}
	return false;
}

void GUIParticleSelector::setParticleVectorPtr(std::vector<Particle>* particleVectorPtr)
{
	m_particleVectorPtr = particleVectorPtr;
}

Particle * GUIParticleSelector::m_getSelectedParticle(unsigned int mouseX, unsigned int mouseY)
{
	float closestDist = m_distSquared;
	Particle* closestParticle = nullptr;
	float x = mouseX, y = mouseY;

	for (auto& p : *m_particleVectorPtr)
	{
		float dx = p.x - x;
		float dy = p.y - y;
		float dist = dx * dx + dy * dy;
		if (dist <= m_distSquared && dist < closestDist)
		{
			closestDist = dist;
			closestParticle = &p;
		} 
	}
	return closestParticle;
}
