#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <vector>
#include <random>

struct Particle
{
	Particle() {};
	Particle(float _x, float _y, float _vx, float _vy, uint8_t _type) : x(_x), y(_y), vx(_vx), vy(_vy), type(_type) {};
	float x, y, vx, vy;
	uint8_t type;
};

class ParticleTypes
{
public:
	ParticleTypes();
	void resize(std::size_t size);
	size_t size() const { return m_typeCount; }
	const sf::Color& color(std::size_t i) const { return m_color[i]; }

	void addRandomTypes(std::size_t typeCount);

	float minRadius(std::size_t typeA, std::size_t typeB);
	float maxRadius(std::size_t typeA, std::size_t typeB);
	float attractForce(std::size_t typeA, std::size_t typeB);
	unsigned int getTypeCount() const;
	void setParams(float attract_mean, float attract_std, float minr_lower, float minr_upper, float maxr_lower, float maxr_upper, float friction);
private:
	std::size_t m_typeCount;
	std::vector<Particle> m_particles;
	std::vector<sf::Color> m_color;
	std::vector<float> m_minRad;
	std::vector<float> m_maxRad;
	std::vector<float> m_attractForce;

	std::random_device m_rd;
	std::mt19937 m_gen;

	float m_friction;
	float m_attract_mean;
	float m_attract_std;
	float m_minr_lower;
	float m_minr_upper;
	float m_maxr_lower;
	float m_maxr_upper;
};