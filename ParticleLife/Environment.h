#pragma once
#include "Particle.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <cinttypes>
#include <tuple>
#include "BatchRenderer2D.h"

enum BoundaryCollisionType : unsigned int
{
	EMPTY = 0,
	SOLID = 1,
	WRAP =	2,
};

enum DebugDrawConfig : unsigned int
{
	NO_DEBUG_DRAW		= 0x1 << 0,
	MIN_RADIUS			= 0x1 << 1,
	INTERACTION_LINE	= 0x1 << 2,
	MAX_RADIUS			= 0x1 << 3,
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
	void setDebugFlags(unsigned int flags);
	void setDebugFlags(unsigned int flags, bool value);
	void toggleDebugFlag(unsigned int flag);
	void setNeighboorSearchRadius(float radius);
	void setNeighboorSearchRadiusModeAuto(bool enabled);
	void setParams(float attract_mean, float attract_std, float minr_lower, float minr_upper, float maxr_lower, float maxr_upper, float friction, bool flat_force);
	void setDebugDrawingAlpha(unsigned char alpha);

	void drawDebugTextureAtlas(sf::RenderTarget * renderTarget, float x, float y, float width, float height);

private:
	sf::Color m_randomColor();
	std::uniform_int_distribution<unsigned int> m_randByte;

	
	// Debug stuff
	void m_debugDraw(sf::RenderWindow* window);
	sf::VertexArray m_debugInteractionLines;
	bool m_drawDebug;
	unsigned char m_debugDrawingAlpha;
	unsigned int m_debugDrawConf;

	
	unsigned int m_getNeighbours(const Particle& particle, float searchRadius);

	void m_buildRenderBatch(unsigned int textureRes, unsigned int offset = 0);
	void m_updateRenderBatchPos();



	BoundaryCollisionType m_bndColTy;
	float m_width, m_height, m_boundWidth, m_boundHeight;
	std::size_t m_particleCount;
	std::vector<Particle> m_particles;
	std::vector<std::pair<Particle*, float>> m_neighboorPtrBuffer;
	std::size_t m_lastNeighboorScanCount;
	ParticleTypes m_types;

	bool m_useFlatForce;
	float m_friction;
	float m_attract_mean;
	float m_attract_std;
	float m_minr_lower;
	float m_minr_upper;
	float m_maxr_lower;
	float m_maxr_upper;
	float m_neighboorSearchRadius;
	bool m_autoSearchNeighboorRadius;


	std::vector<std::pair<float, float>> m_debugDrawingRadii;

	std::random_device m_rd;
	std::mt19937 m_gen;

	BatchRenderer2D m_batchRenderer;
};
