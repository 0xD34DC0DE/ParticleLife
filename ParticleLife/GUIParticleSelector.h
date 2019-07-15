#pragma once
#include "GUI.h"
#include "Particle.h"

class GUIParticleSelector : public GUI<EventType::MOUSE_CLICK>
{
public:
	GUIParticleSelector(float radius, unsigned int textureRes, std::vector<Particle>* particleVectorPtr, sf::Color highlighColor = sf::Color::Yellow);
	void draw(sf::RenderTarget* renderTarget) override final;
	bool update(sf::Event evnt) override final;
	void setParticleVectorPtr(std::vector<Particle>* particleVectorPtr);
private:
	std::vector<Particle>* m_particleVectorPtr;
	float m_radius;
	unsigned int m_distSquared;
	unsigned int textureRes;
	float m_maxSelectDist;

	bool m_particleSelected;

	//return nullptr if no particle got selected
	Particle* m_getSelectedParticle(unsigned int x, unsigned int y);
};

