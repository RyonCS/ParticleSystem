#pragma once

#include <glm/gtc/matrix_transform.hpp> 
#include "glm/glm.hpp"
#include <glm/gtc/random.hpp>
#include <glad/glad.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include "Particle.hpp"

class ParticleEmitter {
    public:
        ParticleEmitter();
        ~ParticleEmitter();

        void InitializeBuffers();

        int FindUnusedParticle();

        void GenerateRandomParticles(int numParticles);

        void UpdateParticles(bool frustumCulling);

        void RenderParticles();

        glm::mat4 GetModelMatrix() {
            return m_modelMatrix;
        }

        glm::vec3 GetPosition() {
            return m_emitterPosition;
        }

        void SortParticles();

        void GetFrustumPlanes(const glm::mat4& viewProjectionMatrix);

        bool ParticleFrustumCheck(const glm::vec3& position);

        int GetNumParticlesRendered();

        void increaseGravity() {
            m_gravity.y += 1;
        }

        void decreaseGravity() {
            m_gravity.y -= 1;
        }

        void increaseSpread() {
            m_spread += .1;
        }

        void decreaseSpread() {
            m_spread = std::max(0.0f, m_spread - .1f);
        }

    private:
        glm::vec3 m_emitterPosition = glm::vec3(0.0f);
        Particle m_particles[100000];
        int m_maxParticles = 100000;
        int m_lastUsedParticle = 0;
        int m_particleRenderCount = 0;
        float deltaTime = 0;
        glm::mat4 m_viewProjectionMatrix;

        GLuint m_VAO, m_VBO, m_instanceVBO;
        GLuint m_positionBuffer, m_colorBuffer;
        GLuint m_shaderProgram;
        glm::vec3 m_gravity = glm::vec3(0.0f, -10.5f, 0.0f);
        float m_spread = 2.0f;

        glm::vec4 m_frustumPlanes[6];

        glm::mat4 m_modelMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,-5.0f));
};