#pragma once

#include <glm/gtc/matrix_transform.hpp>

// we will use a blend kernel for different forces in order to get a stable simulation
struct SphKernel2 {
	float h; // h is the support radius
	float poly6coeff;
	float spikycoeff;
	float viscoscoeff;

	inline explicit SphKernel2(float supportRadius) : h(supportRadius),
	  poly6coeff(4.0f / (glm::pi<float>() * std::powf(h,8.0f))),
	  spikycoeff(10.0f / (glm::pi<float>() * std::powf(h,5.0f))),
	  viscoscoeff(40.0f / (glm::pi<float>() * std::powf(h,5.0f))) {};
	
	SphKernel2() = delete;
	//override the operator to return the value at give radius
	inline float operator()(float distance) const {
		if (distance >= 0.0f && distance <= h){
			return poly6coeff * std::powf((h * h - distance * distance), 3.0f);
		}
		else
			return 0.0f;
	};

	// for numeric value of the gradient
	inline float firstDerivative(float distance) const {
		if (distance > 0.0f && distance <= h)
			return spikycoeff * std::powf(h - distance, 3.0f);
		else
			return 0.0f;
	};
	// real gradient vector of \nabla W_ij
	inline glm::vec2 gradient(const glm::vec2& vector2Center) const {
		float distance = glm::length(vector2Center);
		return vector2Center * firstDerivative(distance) / distance;
	};

	inline float secondDerivative(float distance) const {
		if (distance > 0.0f && distance <= h)
			return viscoscoeff * (h - distance);
		else
			return 0.0f;
	}

	inline float laplacian(const glm::vec2& vector2Center) const {
		float distance = glm::length(vector2Center);
		return secondDerivative(distance);
	}
};