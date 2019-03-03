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
private:
	std::size_t m_typeCount;
	std::vector<Particle> m_particles;
	std::vector<sf::Color> m_color;

	std::random_device m_rd;
	std::mt19937 m_gen;
};