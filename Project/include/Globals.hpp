//Globals.hpp - Header file for our globals. Globals are declared here.
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

// C++ Standard Libraries
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "../include/Startup/Shader.hpp"
#include "Camera.hpp"

struct Global {

    int gWindowWidth = 640;
    int gWindowHeight = 480;

    bool gQuit;

    Camera gCamera;

    GLuint gGraphicsPipelineShaderProgram;
    GLenum gPolygonMode = GL_LINE;
};

extern Global g;