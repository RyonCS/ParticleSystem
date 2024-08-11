// Main.cpp - Entry point into program.
#include "../include/Startup/Shader.hpp"
#include "Globals.hpp"

#include <SDL2/SDL.h>
#include "../include/Startup/SDLGraphicsProgram.hpp"

// -- ENTRY POINT --
int main(int argc, char* argcv[]) {
    SDLGraphicsProgram program(640, 480);

    program.Loop();

    return 0;
}