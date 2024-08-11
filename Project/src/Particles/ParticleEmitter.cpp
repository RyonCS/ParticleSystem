// ParticleEmitter.hpp - Header file for the particle emitter class.

#include <algorithm>
#include <chrono>
#include <fstream>
#include <cmath>

#include "../include/Particles/ParticleEmitter.hpp"
#include "../include/Startup/Shader.hpp"
#include "Globals.hpp"

using namespace std::chrono;
steady_clock::time_point lastTime = steady_clock::now();

/**
 * Constructor - creates a particle shader program, sets up buffers, and initializes particle values.
 */
ParticleEmitter::ParticleEmitter() {
    // Create a new shader program for rendering particles.
    Shader* particleShader = new Shader();
    std::string vertexShader = particleShader->LoadShaderAsString("./shaders/Particle.vert");
    std::string fragmentShader = particleShader->LoadShaderAsString("./shaders/Particle.frag");
    particleShader->CreateShaderProgram(vertexShader, fragmentShader);     
    m_shaderProgram = particleShader->GetShaderID();  

    // Initialize particle buffers.
    InitializeBuffers();

    // Set all particles to negative life and camera distance.
    for(int i=0; i<m_maxParticles; i++){
		m_particles[i].life = -1.0f;
		m_particles[i].cameraDistance = -1.0f;
	}
}

/**
 * Destructor - Delete VAO, VBOs, and graphics pipeline.
 */
ParticleEmitter::~ParticleEmitter() {
    if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO) glDeleteBuffers(1, &m_VBO);
    if (m_positionBuffer) glDeleteBuffers(1, &m_positionBuffer);
    if (m_colorBuffer) glDeleteBuffers(1, &m_colorBuffer);
    if (m_shaderProgram) glDeleteProgram (m_shaderProgram);
}

/**
 * Declares a quad shape and creates a VAO, position, and color buffer.
 */
void ParticleEmitter::InitializeBuffers() {
    // Create Vertex Array Object.
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Declare two triangle vertices to make a quad.
    static const GLfloat vertexData[] = {
    -0.5f, -0.5f, 0.0f, // T1
     0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f, // T2
     0.5f, -0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
    };

    // Create a Vertex Buffer Object for quad data.
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    // Create a Vertex Buffer Object for particle positions.
    glGenBuffers(1, &m_positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

    // Create a Vertex Buffer Object for particle colors.
    glGenBuffers(1, &m_colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_maxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

    // Declare vertex attributes - quad vertices.
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // Declare vertex attributes - particle positions.
    glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    // Declare vertex attributes - particle colors.
    glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);
    glEnableVertexAttribArray(2);

    // Unbind the VAO
    glBindVertexArray(0);
}

/**
 * Iterate through the array of particles to find the first dead particle.
 */
int ParticleEmitter::FindUnusedParticle() {
    // Try to find an unused particle from the last used index.
    // This will shorten the search time.
    for (int i = m_lastUsedParticle; i < m_maxParticles; i++) {
        if (m_particles[i].life <= 0.0) {
            m_lastUsedParticle = i;
            return i;
        }
    }

    // If no particle was found from the last used...
    // Conduct a linear search from the beginning of the array.
    for (int i = 0; i < m_lastUsedParticle; i++) {
        if (m_particles[i].life <= 0.0) {
            m_lastUsedParticle = i;
            return i;
        }
    }

    // If all particles are alive, overwrite the first particle.
    m_lastUsedParticle = 0;
    return 0; 
}

/**
 * Generates random particle values for each particle based on the number of new particles to render.
 */
void ParticleEmitter::GenerateRandomParticles(int numParticles) {
        // Iterate through particles to generate new ones with random attributes.
    for (int i = 0; i < numParticles; i++){

        // Try to find first dead one to replace or first particle in array.
        int particleIndex = FindUnusedParticle();

        // Life attribute - random number between 
        m_particles[particleIndex].life = glm::linearRand(0.5f, 5.0f); // This particle will live 5 seconds.
        m_particles[particleIndex].pos = glm::vec3(0,0,0);

        // Set the initial direction to upward to get the fountain effect.
        glm::vec3 initialDirection = glm::vec3(0.0f, 10.0f, 0.0f);

        // Create a random direction for all of the particles.
        glm::vec3 randomDirection = glm::vec3(
            glm::linearRand(-1.0f, 1.0f),
            glm::linearRand(-1.0f, 1.0f),
            glm::linearRand(-1.0f, 1.0f)
        );
        
        // Calculate particle speed.
        m_particles[particleIndex].speed = initialDirection + randomDirection * m_spread;

        // Generate Random Particle colors.
        m_particles[particleIndex].r = glm::linearRand(0.0f, 256.0f);
        m_particles[particleIndex].g = glm::linearRand(0.0f, 256.0f);
        m_particles[particleIndex].b = glm::linearRand(0.0f, 256.0f);
        m_particles[particleIndex].a = glm::linearRand(0.0f, 256.0f) / 3;

        m_particles[particleIndex].size = glm::linearRand(0.1f, 0.6f);
    }
}

/**
 * Generates new particles each frame and updates the positions of the particles based on gravity and spread.
 * 
 * @param frustumCulling - A boolean value used to decide if frustum culling is turned on/off.
 */
void ParticleEmitter::UpdateParticles(bool frustumCulling) {
    // Calculate the view-projection matrix for frustum culling.
    glm::mat4 viewMatrix = g.gCamera.GetViewMatrix();
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(75.0f), 
                        (float)g.gWindowWidth / (float)g.gWindowHeight, 1.0f, 75.0f);
    glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;

    // Get the frustum planes.
    GetFrustumPlanes(viewProjectionMatrix);

    // Calculate delta time since last update.
    steady_clock::time_point currentTime = steady_clock::now();
    duration<float> deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    float m_deltaTime = deltaTime.count();

    // Retrieve the camera position.
    glm::vec3 cameraPosition = g.gCamera.GetCameraPosition();

    // Calculate the number of new particles to emit based on the elapsed time.
    int newparticles = (int)(m_deltaTime * 10000.0);
    if (newparticles > (int)(0.016f * 10000.0))
        newparticles = (int)(0.016f * 10000.0);

    // Create new particles to replace dead ones.
    GenerateRandomParticles(newparticles);

    // Prepare buffers for new particle data.
    std::vector<float> gpuParticleData(m_maxParticles * 4, 0.0f);
    std::vector<unsigned char> gpuParticleColorData(m_maxParticles * 4, 0);

    m_particleRenderCount = 0;

    // Iterate through the particles.
    for (int i = 0; i < m_maxParticles; i++) {
        Particle& p = m_particles[i];

        // Check if the particle is alive.
        if (p.life > 0.0f) {
            // Update particle life.
            p.life -= m_deltaTime;

            // If particle is alive, update its attributes and check frustum culling.
            if (p.life > 0.0f) {
                // Frustum culling on or off depending on boolean value passed in.
                bool isVisible = !frustumCulling || ParticleFrustumCheck(p.pos);
                if (isVisible) {
                    p.speed += m_gravity * m_deltaTime * 0.5f;
                    p.pos += p.speed * m_deltaTime;
                    // Used for sorting the particles by their distance to the camera.
                    p.cameraDistance = glm::length(p.pos - cameraPosition);

                    // Store particle position data for pushing into the GPU.
                    gpuParticleData[4 * m_particleRenderCount + 0] = p.pos.x;
                    gpuParticleData[4 * m_particleRenderCount + 1] = p.pos.y;
                    gpuParticleData[4 * m_particleRenderCount + 2] = p.pos.z;
                    gpuParticleData[4 * m_particleRenderCount + 3] = p.size;

                    // Store particle color data for pushing into the GPU.
                    gpuParticleColorData[4 * m_particleRenderCount + 0] = p.r;
                    gpuParticleColorData[4 * m_particleRenderCount + 1] = p.g;
                    gpuParticleColorData[4 * m_particleRenderCount + 2] = p.b;
                    gpuParticleColorData[4 * m_particleRenderCount + 3] = p.a;

                    m_particleRenderCount++;
                } else {
                    // If the particle is not visible it is sent to the back of the sorted array.
                    p.cameraDistance = -1.0f;
                }
            } else {
                // If the particle is dead it is sent to the back of the sorted array.
                p.cameraDistance = -1.0f;
            }
        }
    }

    // Sort particles from furthest to closest to the camera.
    SortParticles();

    // Update GPU buffers with new position and color data.
    glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_particleRenderCount * 4 * sizeof(float), gpuParticleData.data());

    glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_particleRenderCount * 4 * sizeof(unsigned char), gpuParticleColorData.data());
}


/**
 * Render particles.
 */
void ParticleEmitter::RenderParticles() {
    // Clear the color and depth buffers to prepare for a new frame.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

    // Enables blending for transparent objects based on alpha value.
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Send model matrix uniform to shader.
    glm::mat4 model = GetModelMatrix();
    GLint u_ModelMatrixLocation = glGetUniformLocation(m_shaderProgram, "u_ModelMatrix");
    glUniformMatrix4fv(u_ModelMatrixLocation, 1, GL_FALSE, &model[0][0]);

    // Send view matrix to shader.
    GLint u_ViewLocation = glGetUniformLocation(m_shaderProgram, "u_ViewMatrix");
    glm::mat4 viewMatrix = g.gCamera.GetViewMatrix();
    glUniformMatrix4fv(u_ViewLocation, 1, GL_FALSE, &viewMatrix[0][0]);

    // Send projection matrix to shader.
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)g.gWindowWidth / (float)g.gWindowHeight, 0.1f, 50.0f);
    GLint u_ProjectionLocation = glGetUniformLocation(m_shaderProgram, "u_ProjectionMatrix");
    glUniformMatrix4fv(u_ProjectionLocation, 1, GL_FALSE, &projection[0][0]);

    // Use shader program.
    glUseProgram(m_shaderProgram);

    // Bind VAO.
    glBindVertexArray(m_VAO);

    // Set attribute divisors which allow for instancing.
    glVertexAttribDivisor(0, 0); // Quad vertices - same per instance.
    glVertexAttribDivisor(1, 1); // Particle positions - advance once per instance.
    glVertexAttribDivisor(2, 1); // Particle colors - advance once per instance.

    // Draw instanced quads.
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, m_particleRenderCount);

    // Unbind VAO.
    glBindVertexArray(0);
}

/**
 * Sorts particles in order of furthest to closest.
 */
void ParticleEmitter::SortParticles(){
	std::sort(&m_particles[0], &m_particles[m_maxParticles]);
}

/**
 * Calculate the view frustum's 6 planes using the VP matrix.
 */
void ParticleEmitter::GetFrustumPlanes(const glm::mat4& viewProjectionMatrix) {
    glm::vec4 row0 = viewProjectionMatrix[0]; //x
    glm::vec4 row1 = viewProjectionMatrix[1]; //y
    glm::vec4 row2 = viewProjectionMatrix[2]; //z
    glm::vec4 row3 = viewProjectionMatrix[3]; //w

    // Calculate frustum planes.
    m_frustumPlanes[0] = row3 + row0; // Right Plane
    m_frustumPlanes[1] = row3 - row0; // Left Plane
    m_frustumPlanes[2] = row3 + row1; // Top Plane
    m_frustumPlanes[3] = row3 - row1; // Bottom Plane
    m_frustumPlanes[4] = row3 + row2; // Far Plane
    m_frustumPlanes[5] = row3 - row2; // Near Plane

    // Normalize planes.
    for (int i = 0; i < 6; ++i) {
        m_frustumPlanes[i] = glm::normalize(m_frustumPlanes[i]);
    }
}

/**
 * Check whether the particle's position is in the view freustum.
 */
bool ParticleEmitter::ParticleFrustumCheck(const glm::vec3& position) {
    // Iterate through each plane and check if the particle's position is on the positive side of the plane.
    for (int i = 0; i < 6; ++i) {
        if (glm::dot(glm::vec3(m_frustumPlanes[i]), position) + m_frustumPlanes[i].w < 0) {
            return false;
        }
    }
    return true;
}

/**
 * Return the number of particles rendered.
 */
int ParticleEmitter::GetNumParticlesRendered() {
    return m_particleRenderCount;
}