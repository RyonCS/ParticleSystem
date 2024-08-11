#include <SDL2/SDL.h>

#include <iostream>
#include <iomanip>

#include "../../include/Startup/SDLGraphicsProgram.hpp"
#include "Globals.hpp"

Uint32 previousTime = 0;

/**
 * Initializes our graphics program by creating a window, OpenGLContext, and a renderer.
 */
SDLGraphicsProgram::SDLGraphicsProgram(int windowHeight, int windowWidth) {
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize. SDL Error: " << SDL_GetError() <<std::endl;
    }

    // Setup OpenGL Context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create SDL window with OpenGL.
    m_window = SDL_CreateWindow("Particle Emitter",
                                                   SDL_WINDOWPOS_UNDEFINED,
                                                   SDL_WINDOWPOS_UNDEFINED,
                                                   m_windowHeight,
                                                   m_windowWidth,
                                                   SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    // Check if window was properly initialized.
    if (nullptr == m_window) {
        std::cout << "SDL window could not be ce SDL Error" << SDL_GetError() << std::endl;
        exit(1);
    }
    
    // Create OpenGL Context.
    m_openGLContext = SDL_GL_CreateContext(m_window);
    if(nullptr == m_openGLContext) {
        std::cout << "OpenGL Context could not be created. SDL error: " << SDL_GetError() << std::endl;
    }

    // Initialize GLAD library.
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        std::cout << "GLAD did not initialize" << std::endl;
        exit(1);
    }
    
    // ObjectManager* objectManager = new ObjectManager();
    // RenderingManager* renderingManager = new RenderingManager(objectManager);
    // m_renderingManager = renderingManager;
    m_particleEmitter = new ParticleEmitter();

    g.gCamera.SetCameraEyePosition(0.0, 5.0, 25.0f);
}

/**
 * Destructs the SDL window and quits SDL.
 */
SDLGraphicsProgram::~SDLGraphicsProgram() {   
    SDL_DestroyWindow(m_window);
    m_window = nullptr;

    SDL_Quit();
}

/**
 * Gets the SDL window width.
 * 
 * @return the width of the SDL window.
 */
int SDLGraphicsProgram::GetWindowWidth() {
    return m_windowWidth;
}

/**
 * Gets the SDL window height.
 * 
 * @return the height of the SDL window.
 */
int SDLGraphicsProgram::GetWindowHeight() {
    return m_windowHeight;
}

/**
 * Sloppy global objects until adding rendering manager and object manager.
 */


SDL_Window* SDLGraphicsProgram::GetSDLWindow() {
    return m_window;
}

/**
 * Gets input from the user.
 */
void SDLGraphicsProgram::Input() {
    static int mouseX = m_windowWidth/2;
    static int mouseY = m_windowHeight/2;

    SDL_Event event;
    while(SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            m_quit = true;
        }
        // Allows us to quit using "ESC".
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            m_quit = true;
        } 
        if(event.type==SDL_MOUSEMOTION){
            // Capture the change in the mouse position
            mouseX+=event.motion.xrel;
            mouseY+=event.motion.yrel;
            g.gCamera.MouseLook(mouseX,mouseY);
        }
    }

    // Retrieve keyboard state.
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    // Forward Camera.
    if (state[SDL_SCANCODE_W]) {
        g.gCamera.MoveForward(0.25f);
    }
    // Backward Camera.
    if (state[SDL_SCANCODE_S]){
        g.gCamera.MoveBackward(0.25f);
    }
    // Left Camera.
    if (state[SDL_SCANCODE_A]) {
        g.gCamera.MoveLeft(0.25f);
    }
    // Right Camera.
    if (state[SDL_SCANCODE_D]) {
        g.gCamera.MoveRight(0.25f);
    }
    // Up Camera.
    if (state[SDL_SCANCODE_UP]) {
        g.gCamera.MoveUp(0.25f);
    }
    // Right Camera.
    if (state[SDL_SCANCODE_DOWN]) {
        g.gCamera.MoveDown(0.25f);
    }

    if (state[SDL_SCANCODE_1]) {
        m_frustumCullingStatus = !m_frustumCullingStatus;
    }
    if (state[SDL_SCANCODE_2]) {
        m_particleEmitter->increaseGravity();
    }

    if (state[SDL_SCANCODE_3]) {
        m_particleEmitter->decreaseGravity();
    }

    if (state[SDL_SCANCODE_4]) {
        m_particleEmitter->increaseSpread();
    }

    if (state[SDL_SCANCODE_5]) {
        m_particleEmitter->decreaseSpread();
    }
}

/** 
 * Loops through the program until the user ends it with esc or presses the red x.
 */
void SDLGraphicsProgram::Loop() {
    Uint32 frameStart;
    float fps = 0.0f;

    SDL_WarpMouseInWindow(m_window, m_windowWidth/2, m_windowHeight/2);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    while (!m_quit) {
        frameStart = SDL_GetTicks();

        // Calculate delta time.
        Uint32 deltaTime = frameStart - previousTime;

        // Process input.
        Input();

        // Update particles and render.
        m_particleEmitter->UpdateParticles(m_frustumCullingStatus);
        m_particleEmitter->RenderParticles();
        int numParticlesRendered = m_particleEmitter->GetNumParticlesRendered();

        // Calculate FPS.
        if (deltaTime > 0) {
            fps = 1000.0f / deltaTime;
        }

        // Update window title with current FPS and particles rendered.
        std::ostringstream oss;
        oss << "Particle Emitter FPS: " << fps
            << " Particles Rendered: " << numParticlesRendered;
        std::string newTitle = oss.str();
        SDL_SetWindowTitle(m_window, newTitle.c_str());

        // Swap buffers
        SDL_GL_SwapWindow(m_window);

        // Update previous time
        previousTime = frameStart;  
    }
}