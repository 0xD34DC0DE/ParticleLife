#include "GUIParticleSelector.h"

//TODO : implement selection distance tolerance
//TODO : implement semi-transparent background circle
//TODO : implement contrasting color circle
GUIParticleSelector::GUIParticleSelector(float radius, unsigned int textureRes, std::vector<Particle>* particleVectorPtr, sf::Color highlighColor)
{
	m_particleVectorPtr = particleVectorPtr;
	m_radius = radius;
	m_distSquared = static_cast<unsigned int>(radius * radius);

	sf::CircleShape circleShape(radius);
	circleShape.setOutlineColor(highlighColor);
	circleShape.setOutlineThickness(2.0f);
	circleShape.setFillColor(sf::Color::Transparent);

	m_batchRenderer.addTexture(circleShape, static_cast<unsigned int>(std::abs(2.0f + radius*2.0f)));
	m_batchRenderer.addSprite(0.0f, 0.0f, 2.0f + radius, 0);

	m_select_x = 0.0f;
	m_select_y = 0.0f;
}

void GUIParticleSelector::draw(sf::RenderTarget* renderTarget)
{
	// Skips drawing if no particle is selected
	if (m_particleSelected)
	{
		m_batchRenderer.setPos(0, m_selectedParticlePtr->x, m_selectedParticlePtr->y);

		m_batchRenderer.draw(renderTarget);
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
			m_selectedParticlePtr = closestParticle;
		}

		// doesn't need coordinate check for the mouse click  since this encapsulate the whole window
		return true;
	}
	else if (evnt.mouseButton.button == sf::Mouse::Button::Right && evnt.type == sf::Event::EventType::MouseButtonReleased)
	{
		m_selectedParticlePtr = nullptr;
		m_particleSelected = false;
	}
	return false;
}

void GUIParticleSelector::setParticleVectorPtr(std::vector<Particle>* particleVectorPtr)
{
	m_particleVectorPtr = particleVectorPtr;
}

Particle* GUIParticleSelector::m_getSelectedParticle(unsigned int mouseX, unsigned int mouseY)
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
