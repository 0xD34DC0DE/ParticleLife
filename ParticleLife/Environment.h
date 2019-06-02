#pragma once
#include "Particle.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <cinttypes>
#include <tuple>

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
	void addRandomTypes(std::size_t typeCount);
	void setBoundaryCollisionType(BoundaryCollisionType bndColTy);
	void setDebugDrawing(bool enabled);
private:
	
	bool m_drawDebug;
	unsigned int m_getNeighbours(const Particle& particle, float searchRadius);

	BoundaryCollisionType m_bndColTy;
	float m_width, m_height, m_boundWidth, m_boundHeight;
	std::size_t m_particleCount;
	std::vector<Particle> m_particles;
	std::vector<std::pair<Particle*, float>> m_neighboorPtrBuffer;
	ParticleTypes m_types;

	std::random_device m_rd;
	std::mt19937 m_gen;


};
