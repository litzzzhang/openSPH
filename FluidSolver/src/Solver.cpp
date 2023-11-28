#include "Solver.h"
#include <iostream>

Solver::Solver(ParticleSystem& particlesystem)
: ParticleSystem_(particlesystem), Kernel_(SphKernel2(particlesystem.SupportRadius_)) {
}

void Solver::Update()
{
	UpdatePressureAndDensity();
	UpdateNonPressureAcceleration();
	UpdatePressureAcceleration();
	Integrate();
	EnforceBoundary();
}

void Solver::Integrate()
{
	// there is nothing but sympletic Euler
	for (int i = 0; i < ParticleSystem_.ParticleCount_; i++) {
		glm::vec2& velocity = ParticleSystem_.Velocity_[i];
		velocity *= 0.9999f;
		velocity += ParticleSystem_.Acceleration_[i] * Global::dt;
		velocity = glm::clamp(velocity, glm::vec2(-100.0f), glm::vec2(100.0f));
		ParticleSystem_.Position_[i] += velocity * Global::dt;
	}
}

void Solver::EnforceBoundary()
{
	float boundDamping = 0.9f;
	for (int i = 0; i < ParticleSystem_.ParticleCount_; i++) {
		glm::vec2& position = ParticleSystem_.Position_[i];
		glm::vec2& velocity = ParticleSystem_.Velocity_[i];
		int DetectCollision = 0;
		if (position.x + ParticleSystem_.SupportRadius_ > ParticleSystem_.UpperRightCorner_.x) {
			DetectCollision = 1;
			velocity.x = -std::abs(velocity.x);
			velocity *= boundDamping;
		}
		if (position.x - ParticleSystem_.SupportRadius_ < ParticleSystem_.LowerLeftCorner_.x) {
			DetectCollision = 1;
			velocity.x = std::abs(velocity.x);
			velocity *= boundDamping;
		}
		if (position.y - ParticleSystem_.SupportRadius_ < ParticleSystem_.LowerLeftCorner_.y) {
			DetectCollision = 1;
			velocity.y = std::abs(velocity.y);
			velocity *= boundDamping;
		}
		if (position.y + ParticleSystem_.SupportRadius_ > ParticleSystem_.UpperRightCorner_.y) {
			DetectCollision = 1;
			velocity.y = -std::abs(velocity.y);
			velocity *= boundDamping;
		}
		if (DetectCollision) {
			position += Global::dt * velocity;
			velocity = glm::clamp(velocity, glm::vec2(-100.0f), glm::vec2(100.0f));
		}
	}

}

void Solver::UpdatePressureAndDensity()
{
	ParticleSystem_.Density_ = std::vector<float>(ParticleSystem_.ParticleCount_, Global::density);
	ParticleSystem_.Pressure_ = std::vector<float>(ParticleSystem_.ParticleCount_, 0.0f);
	for (int i = 0; i < ParticleSystem_.ParticleCount_; i++) {
		if (!ParticleSystem_.Neighbors_[i].empty()) {
			NeighborList& neighbors = ParticleSystem_.Neighbors_[i];
			float density = 0.0f;
			for (const auto& neighbor : neighbors) {
				float estimate = Kernel_(neighbor.distance);
				density += estimate;
			}
			density *= ParticleSystem_.ParticleMass_;
			ParticleSystem_.Density_[i] = std::max(density,Global::density);
		}
		ParticleSystem_.Pressure_[i] = Global::stiffness * 
			(std::powf(ParticleSystem_.Density_[i] / Global::density, Global::exponent) - 1);
	}

}

void Solver::UpdatePressureAcceleration()
{
	for (int i = 0; i < ParticleSystem_.ParticleCount_; i++) {
		if (!ParticleSystem_.Neighbors_[i].empty()) {
			NeighborList& neighbors = ParticleSystem_.Neighbors_[i];
			std::vector<float>& pressure = ParticleSystem_.Pressure_;
			glm::vec2 pressureAcceleration = glm::vec2(0.0f);

			for (const auto& neighbor : neighbors) {
				int j = neighbor.index;
				pressureAcceleration += (pressure[i] + pressure[j]) / (2.0f * ParticleSystem_.Density_[j]) *
					-Kernel_.gradient(neighbor.dirVector);
			}
			pressureAcceleration *= ParticleSystem_.ParticleMass_;
			ParticleSystem_.Acceleration_[i] -= pressureAcceleration;
			/*if (ParticleSystem_.Acceleration_[i].x < -7000.0f) {
				std::cout << "acceleration explode" << std::endl;
			}*/
		}
	}
	
}

void Solver::UpdateNonPressureAcceleration()
{
	// Add gravity force
	///ParticleSystem_.Acceleration_.assign(ParticleSystem_.ParticleCount_, glm::vec2(0.0f));
	ParticleSystem_.Acceleration_.assign(ParticleSystem_.ParticleCount_, Global::gravity);

	// apply acceleration caused by viscous
	float coeff = Global::viscosity * ParticleSystem_.ParticleMass_;
	// coefficient = 2 * (d + 2) * \nu * mass

	for (int i = 0; i < ParticleSystem_.ParticleCount_; i++) {
		if (!ParticleSystem_.Neighbors_[i].empty()) {
			NeighborList& neighbors = ParticleSystem_.Neighbors_[i];

			glm::vec2 viscousAcceleration = glm::vec2(0.0f);
			for (const auto& neighbor : neighbors) {
				const int& j = neighbor.index;
				glm::vec2 v_ji = ParticleSystem_.Velocity_[j] - ParticleSystem_.Velocity_[i];
				viscousAcceleration += v_ji * (Kernel_.secondDerivative(neighbor.distance) / ParticleSystem_.Density_[j]);
			}
			viscousAcceleration *= (coeff / ParticleSystem_.Density_[i]);
			ParticleSystem_.Acceleration_[i] += viscousAcceleration;
			/*if (ParticleSystem_.Acceleration_[i].x < -7000.0f) {
				std::cout << "acceleration explode" << std::endl;
			}*/
		}
	}
}

