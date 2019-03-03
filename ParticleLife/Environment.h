#pragma once
#include "Particle.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <cinttypes>

enum BoundaryCollisionType
{
	EMPTY = 0,
	SOLID = 1,
	WRAP =	2,
};

class Environment
{
public:
	Environment(unsigned int width, unsigned int height);
	void update();
	void draw(sf::RenderWindow* window);
	void createRandomParticles(std::size_t particleCount, float velMean, float velStd);
	void createRandomTypes(std::size_t typeCount);
	void setBoundaryCollisionType(BoundaryCollisionType bndColTy);
private:
	BoundaryCollisionType m_bndColTy;
	float m_width, m_height, m_boundWidth, m_boundHeight;
	std::size_t m_particleCount;
	std::vector<Particle> m_particles;
	ParticleTypes m_types;

	std::random_device m_rd;
	std::mt19937 m_gen;

};
