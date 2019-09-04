#pragma once
#include "GUI.h"
#include "Particle.h"
#include "BatchRenderer2D.h"

class GUIParticleSelector : public GUI<static_cast<EventType>(EventType::MOUSE_CLICK | EventType::MOUSE_MOVE | EventType::WINDOW_RESIZE)>
{
public:
	GUIParticleSelector(sf::Vector2u environmentSize, sf::Vector2u windowSize, float radius, float tolerance, float backgroundHighlight, unsigned int textureRes, std::vector<Particle>* particleVectorPtr, sf::Color highlighColor = sf::Color::Yellow, sf::Color backCircleColor = sf::Color(0xFF,0xFF,0xFF,0x80));
	void draw(sf::RenderTarget* renderTarget) override final;
	bool update(sf::Event evnt, CustomData* data) override final;
	void setParticleVectorPtr(std::vector<Particle>* particleVectorPtr);
	void setWindowSize(sf::Vector2u windowSize);
private:
	sf::Vector2f m_enviromentSize;
	sf::Vector2f m_windowSize;
	std::vector<Particle>* m_particleVectorPtr;
	float m_radius;
	float m_tolerance;
	float m_backgroundHighlightRadius;
	unsigned int m_distSquared;
	unsigned int m_textureRes;
	unsigned int m_toleranceDistSquared;
	float m_maxSelectDist;

	bool m_particleSelected;
	float m_select_x, m_select_y;

	//return nullptr if no particle got selected
	Particle* m_getSelectedParticle(unsigned int x, unsigned int y);
	Particle* m_selectedParticlePtr;

	BatchRenderer2D m_batchRenderer;
	unsigned int m_selectedTextureID;
	unsigned int m_preSelectTextureID;
};

