// Shader.hpp - Header file for our shader class.
#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
    public:
        Shader();

        ~Shader();

        void Bind() const;

        /**
         * Parses a shader file and returns a string representation.
         * 
         * @param fileName - the path to the shader file.
         * @return - the shader file in string format.
         */
        std::string LoadShaderAsString(const std::string &fileName);

        /**
         * Compiles the shader depending on its type and returns a shader object.
         * 
         * @param type - the type of shader (i.e. vertex or fragment.)
         * @param source - the string representation of the shader file from LoadShaderAsString(...).
         * @return - a GLuint representing the individual shader object.
         */
        GLuint CompileShader(GLuint type, const std::string &source);

        /**
         * Creates our graphics pipeline by binding our shaders :)
         * 
         * @param vertexShaderSource - the string representation of the shader file from LoadShaderAsString(...).
         * @param fragmentShaderSource - the string representation of the shader file from LoadShaderAsString(...).
         * @return - a GLuint representing our graphics pipeline.
         */
        GLuint CreateShaderProgram(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);

        /**
         * Gets the shaderID.
         * 
         * @return - the shaderID.
         */
        GLuint GetShaderID();

        /**
         * Prints a debugging log about a specific shader.
         * 
         * @param shaderID - the ID of a specific shader.
         */
        void PrintShaderInfoLog(GLuint shaderID);

        /**
         * Prints a debugging log about our graphics pipeline.
         * 
         * @param programObject - the ID of the graphics pipeline.
         */
        void PrintProgramInfoLog(GLuint programObject);

        /**
         * Prints a robust debugging log about our entire graphics pipeline including attached shaders, 
         * active atributes, and uniforms.
         * 
         * @programObject - the ID of the graphics pipeline.
         */
        void PrintAllLogs(GLuint programObject);

        /**
         * Transforms an OpenGL type to a usable string.
         * 
         * @param type - the OpenGL type we want to convert.
         * @return - a string representation of an OpenGL type.
         */
        const char* GLTypeToString(GLenum type);

        void SetUniformMatrix4fv(const GLchar* name, const GLfloat* value);

        void SetUniform3f(const GLchar* name, float r, float g, float b);

        void PrintActiveUniforms(GLuint program);

    private:
        std::string m_vertexString;
        std::string m_fragmentString;

        GLuint m_vertexShader;
        GLuint m_fragmentShader;
        GLuint m_shaderID;
};