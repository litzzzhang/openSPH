#pragma once

#include "ParticleSystem.h"
#include "SphKernel2.h"

class Solver
{
public:
	explicit Solver(ParticleSystem& particlesystem);
	void Update();

private:
	void Integrate();
	void EnforceBoundary();
	void UpdatePressureAndDensity();
	void UpdatePressureAcceleration();
	void UpdateNonPressureAcceleration();

private:
	ParticleSystem& ParticleSystem_;
	SphKernel2 Kernel_;
};

