// Particle.hpp - Header file for Particle Struct.

#pragma once
#include "glm/glm.hpp"


struct Particle {
    glm::vec3 pos, speed;
    unsigned char r, g, b, a;
    float size, angle, weight, life, cameraDistance;
    glm::vec3 velocity;
    bool isVisible = true;

    // Operator for comparing particles when sorting.
    bool operator<(const Particle& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameraDistance > that.cameraDistance;
	}
};