// SDLWindow.hpp - Header file for our SDL window class.
#pragma once

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../include/Particles/ParticleEmitter.hpp"

class SDLGraphicsProgram {
    public:
        SDLGraphicsProgram(int windowHeight, int windowWidth);

        /**
         * Destructs the SDL window and quits SDL.
         */
        ~SDLGraphicsProgram();

        /**
         * Gets the SDL window width.
         * 
         * @return the width of the SDL window.
         */
        int GetWindowHeight();

        /**
         * Gets the SDL window height.
         * 
         * @return the height of the SDL window.
         */
        int GetWindowWidth();

        /** 
         * Returns a pointer to the SDL window.
         * 
         * @return a pointer to the SDL window.
         */
        SDL_Window* GetSDLWindow();

        /**
         * Responds to user input.
         */
        void Input();

        /**
         * Loop that runs forever
         */ 
        void Loop();

    private:
        SDL_Window* m_window = nullptr;
        SDL_GLContext m_openGLContext = nullptr;
        ParticleEmitter *m_particleEmitter;

        bool m_quit = false;
        bool m_frustumCullingStatus = false;

        int m_windowHeight;
        int m_windowWidth;

};