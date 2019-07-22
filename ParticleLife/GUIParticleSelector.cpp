#include "GUIParticleSelector.h"

//TODO : implement selection distance tolerance
//TODO : implement semi-transparent background circle
//TODO : implement contrasting color circle
//TODO : implement option to change background color and outline color when selected
GUIParticleSelector::GUIParticleSelector(float radius, float tolerance, float backgroundHighlight, unsigned int textureRes, std::vector<Particle>* particleVectorPtr, sf::Color highlighColor, sf::Color backCircleColor)
{
	m_particleVectorPtr = particleVectorPtr;
	m_radius = radius;
	m_distSquared = static_cast<unsigned int>(radius * radius);
	m_toleranceDistSquared = static_cast<unsigned int>((radius + tolerance) * (radius + tolerance));
	m_backgroundHighlightRadius = backgroundHighlight + radius;

	sf::CircleShape circleShape(m_backgroundHighlightRadius);
	circleShape.setFillColor(backCircleColor);

	sf::RenderTexture rt;
	unsigned int halfTextureSize = static_cast<unsigned int>(std::abs((m_backgroundHighlightRadius)));
	
	rt.create(halfTextureSize * 2, halfTextureSize * 2);

	rt.clear(sf::Color::Transparent);
	
	rt.draw(circleShape);

	const float selectedOutlineThickness = 2.0f;
	circleShape.setRadius(radius);
	circleShape.setOrigin(radius, radius);
	circleShape.setPosition(halfTextureSize, halfTextureSize);
	circleShape.setOutlineColor(highlighColor);
	circleShape.setOutlineThickness(selectedOutlineThickness);
	circleShape.setFillColor(sf::Color::Transparent);

	rt.draw(circleShape);

	m_selectedTextureID = m_batchRenderer.addTexture(circleShape, static_cast<unsigned int>(std::abs((radius) * 2.0f)));

	m_preSelectTextureID = m_batchRenderer.addTexture(rt.getTexture());
	m_batchRenderer.addSprite(0.0f, 0.0f, m_backgroundHighlightRadius, m_preSelectTextureID);

	m_select_x = 0.0f;
	m_select_y = 0.0f;
}

void GUIParticleSelector::draw(sf::RenderTarget* renderTarget)
{
	// Skips drawing if no particle pointer is available
	if (m_selectedParticlePtr != nullptr)
	{
		m_batchRenderer.setPos(0, m_selectedParticlePtr->x, m_selectedParticlePtr->y, true); // "true" enables the use of the current texture size as quad size
		m_batchRenderer.draw(renderTarget);
	}
}

bool GUIParticleSelector::update(sf::Event evnt, CustomData* data)
{
	if (evnt.type == sf::Event::EventType::MouseButtonReleased && evnt.mouseButton.button == sf::Mouse::Button::Left)
	{
		Particle* closestParticle = m_getSelectedParticle(evnt.mouseButton.x, evnt.mouseButton.y); // will be nullptr if no particle is got selected
		if (closestParticle != nullptr)
		{
			m_particleSelected = true;
			m_selectedParticlePtr = closestParticle;
			m_batchRenderer.setSpriteTexture(0, m_selectedTextureID);
		}

		// doesn't need coordinate check for the mouse click  since this encapsulate the whole window
		return true;
	}
	else if (evnt.type == sf::Event::EventType::MouseButtonReleased && evnt.mouseButton.button == sf::Mouse::Button::Right)
	{
		m_selectedParticlePtr = nullptr;
		m_particleSelected = false;
		m_batchRenderer.setSpriteTexture(0, m_preSelectTextureID);
	}
	else if (evnt.type == sf::Event::EventType::MouseMoved)
	{
		// Do not search for a nearby particle if m_particleSelect is true since we are already tracking one
		if (m_particleSelected != true)
		{
			//Its okay to directly assign the return value since a nullptr means there is no nearby particle and this will mean we skip the drawing
			m_selectedParticlePtr = m_getSelectedParticle(evnt.mouseMove.x, evnt.mouseMove.y);
		}
	}
	return false;
}

void GUIParticleSelector::setParticleVectorPtr(std::vector<Particle>* particleVectorPtr)
{
	m_particleVectorPtr = particleVectorPtr;
}

Particle* GUIParticleSelector::m_getSelectedParticle(unsigned int mouseX, unsigned int mouseY)
{
	float closestDist = m_toleranceDistSquared;
	Particle* closestParticle = nullptr;
	float x = mouseX, y = mouseY;

	for (auto& p : *m_particleVectorPtr)
	{
		float dx = p.x - x;
		float dy = p.y - y;
		float dist = dx * dx + dy * dy;
		if (dist <= m_toleranceDistSquared && dist < closestDist)
		{
			closestDist = dist;
			closestParticle = &p;
		} 
	}
	return closestParticle;
}
