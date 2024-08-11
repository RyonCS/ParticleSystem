#version 450 core

layout (location = 0) in vec3 quadVertices;
layout (location = 1) in vec4 aParticle;
layout (location = 2) in vec4 aColor;

out vec4 fragColor;

uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;
uniform mat4 u_ProjectionMatrix;

void main()
{
    vec3 particlePosition = aParticle.xyz;
    float particleSize = aParticle.w;

    // Scale the quad's vertex position by the particle size
    vec3 scaledVertexPos = quadVertices * particleSize;

    // Move the scaled quad vertex to the particle's position
    vec3 finalPosition = particlePosition + scaledVertexPos;

    // Calculate the final position
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(finalPosition, 1.0);

    fragColor = aColor;
}