Title:
Particle System Using C++ and OpenGL with Optimizations Utilizing Instancing and Frustum Culling

Video:
https://www.youtube.com/watch?v=ZafiFYBXvXM&t=3s

Overview:

This is a final Project for my Advanced Graphics course. Inspired by a Particle System tutorial, I implemented a particle system that continuously renders quads from a particle emitter. The program's implementation is modular so new features can be added. Users can interact in real-time to increase/decrease particle spread and gravity. I optimized the program by implementing instancing which sends a single draw call to the GPU resulting in a reduction of draw calls by 99.99%. Further, I implemented frustum culling utilizing the Gribb-Hartmann method which results in a reduction in particles rendered by 30%.

Resources:
http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/#particle-physics
