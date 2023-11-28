#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

namespace Global {
	const float dt = 0.0002f;
	const float density = 1000.0f;
	// use state equation to estimate the pressure
	const float stiffness = 50.0f;
	const float exponent = 7.0f;
	const float viscosity = 0.05f;
	const glm::vec2 gravity = glm::vec2(0.0f, -9.8f);
}

struct NeighborInfo
{
	int index;
	float distance;
	float distance2;
	glm::vec2 dirVector; // assume particle i and its neighbor j, dirVec = x_i - x_j
};

using NeighborList = std::vector<NeighborInfo>;

class ParticleSystem
{
public:
	int ParticleCount_;
	std::vector<glm::vec2> Position_;
	std::vector<glm::vec2> Velocity_;
	std::vector<glm::vec2> Acceleration_;
	std::vector<float> Density_;
	std::vector<float> Pressure_;
	std::vector<NeighborList> Neighbors_;

	float ParticleRadius_ = 0.005f;
	float SupportRadius_ = 0.020f; // heuristically set to be 4 times of particle radius
	float ParticleVolume_ = glm::pi<float>() * ParticleRadius_ * ParticleRadius_;
	float ParticleMass_ = Global::density * ParticleVolume_;
	
	glm::vec2 LowerLeftCorner_ = glm::vec2(-1.0f, -1.0f);
	glm::vec2 UpperRightCorner_ = glm::vec2(1.0f, 1.0f);
	glm::vec2 SingleGridSize_;

	void SetBoundary(glm::vec2 lowerleftcorner, glm::vec2 size);
	void AddFluidParticles(glm::vec2 lowerleftcorner, glm::vec2 size, glm::vec2 initial_velocity, float resolution1D);
	void SearchNeighbors();
private:
	size_t GridColNum_;
	size_t GridRowNum_;
	size_t GridHashTableSize_ = 4000;
	void BuildHashTable();
	int GetGridHashIDbyPosition(glm::vec2 position);
	int HashFunction(int rowIndex, int columnIndex, int depthIndex);

	std::vector<std::vector<int>> GridHashTable_;
};