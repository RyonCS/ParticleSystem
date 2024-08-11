#include "../../include/Startup/Shader.hpp"

/**
 * Constructor that takes in shader file paths and creates a shader program.
 * 
 */
Shader::Shader() : m_shaderID(0){}

Shader::~Shader() {
    glDeleteProgram(m_shaderID);
}

void Shader::Bind() const {
    glUseProgram(m_shaderID);
}

/**
 * Parses a shader file and returns a string representation.
 * 
 * @param fileName - the path to the shader file.
 * @return - the shader file in string format.
 */
std::string Shader::LoadShaderAsString(const std::string &fileName) {
    // Read in the contents of a shader file.
    std::string result = "";

    std::string line = "";

    std::ifstream myFile(fileName.c_str());

    if (myFile.is_open()) {
        while(std::getline(myFile, line)) {
            result += line + '\n';
        }
        myFile.close();
    }
    return result;
}

/**
 * Compiles the shader depending on its type and returns a shader object.
 * 
 * @param type - the type of shader (i.e. vertex or fragment.)
 * @param source - the string representation of the shader file from LoadShaderAsString(...).
 * @return - a GLuint representing the individual shader object.
 */
GLuint Shader::CompileShader(GLuint type, const std::string &source) {
    // Compile the shader.
    GLuint shaderObject;

    // Based on the type passed in, we create a shader object for that type.
    if (GL_VERTEX_SHADER == type) {
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    } else if (GL_FRAGMENT_SHADER == type) {
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    }

    const char* src = source.c_str();
    // The source of our shader.
    glShaderSource(shaderObject, 1, &src, nullptr);
    // Now we compile.
    glCompileShader(shaderObject);

    // Error Checking!!!
    int result;
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

    if (GL_FALSE == result) {
        int length;
        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
        char* errorMessages = new char[length];
        glGetShaderInfoLog(shaderObject, length, &length, errorMessages);

        if (GL_VERTEX_SHADER == type) {
            std::cout << "Error: GL_VERTEX_SHADER compilation failed: \n" << errorMessages << std::endl;
        } else if (GL_FRAGMENT_SHADER == type) {
            std::cout << "Error: GL_FRAGMENT_SHADER compilation failed: \n" << errorMessages << std::endl;
        }

        // Reclaim memory.
        delete[] errorMessages;

        glDeleteShader(shaderObject);

        return 0;
    }

    return shaderObject;
}

/**
 * Creates our graphics pipeline by binding our shaders :)
 * 
 * @param vertexShaderSource - the string representation of the shader file from LoadShaderAsString(...).
 * @param fragmentShaderSource - the string representation of the shader file from LoadShaderAsString(...).
 * @return - a GLuint representing our graphics pipeline.
 */
GLuint Shader::CreateShaderProgram(const std::string &vertexShaderSource, const std::string &fragmentShaderSource) {
    // Create a new program object.
    GLuint programObject = glCreateProgram();

    // Compile our shaders.
    GLuint myVertexShader   = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    // Link two shaders together.
    glAttachShader(programObject, myVertexShader);
    glAttachShader(programObject, myFragmentShader);
    glLinkProgram(programObject);

    // Check if link was successful.
    int params = -1;
    glGetProgramiv(programObject, GL_LINK_STATUS, &params);
    if (GL_TRUE != params) {
        std::cout << stderr << "ERROR: could not link shader programObject GL index " << programObject << std::endl;
        PrintProgramInfoLog(programObject);
    }

    m_shaderID = programObject;

    // Validate our program.
    glValidateProgram(programObject);

    PrintAllLogs(programObject);

    // Successfully created. Detach and delete shaders.
    glDetachShader(programObject, myVertexShader);
    glDetachShader(programObject, myFragmentShader);

    glDeleteShader(myVertexShader);
    glDeleteShader(myFragmentShader);

    return programObject;
}


/**
 * Gets the shaderID.
 * 
 * @return - the shaderID.
 */
GLuint Shader::GetShaderID() {
    return m_shaderID;
}

/**
 * Prints a debugging log about a specific shader.
 * 
 * @param shaderID - the ID of a specific shader.
 */
void Shader::PrintShaderInfoLog(GLuint shaderID) {
    int max_length = 2048;
    int actual_length = 0;
    char shader_log[2048];
    glGetShaderInfoLog(shaderID, max_length, &actual_length, shader_log);
    std::cout << "Shader info log for GL index: " << shaderID << "\n" << shader_log << std::endl;
}

/**
 * Prints a debugging log about our graphics pipeline.
 * 
 * @param programObject - the ID of the graphics pipeline.
 */
void Shader::PrintProgramInfoLog(GLuint programObject) {
    int max_length = 2048;
    int actual_length = 0;
    char program_log[2048];
    glGetProgramInfoLog(programObject, max_length, &actual_length, program_log);
    std::cout << "Program info log for GL index " << programObject << ": \n" << program_log << std::endl;
}

/**
 * Prints a robust debugging log about our entire graphics pipeline including attached shaders, 
 * active atributes, and uniforms.
 * 
 * @programObject - the ID of the graphics pipeline.
 */
void Shader::PrintAllLogs(GLuint programObject) {
    std::cout << "-----------------\n "<< "Shader Program " << programObject << " info: \n" << std::endl;
    // Check Link Status.
    int params = -1;
    glGetProgramiv(programObject, GL_LINK_STATUS, &params);
    std::cout << "GL_LINK_STATUS = " << params << "\n";

    // Check attached shaders.
    glGetProgramiv(programObject, GL_ATTACHED_SHADERS, &params);
    std::cout << "GL_ATTACHED_SHADERS = " << params << "\n";

    // Check active attributes.
    glGetProgramiv(programObject, GL_ACTIVE_ATTRIBUTES, &params);
    std::cout << "GL_ACTIVE_ATTRIBUTES = " << params << "\n";
    for (int i = 0; i < params; i++) {
        char name[64];
        int max_length = 64;
        int actual_length = 0;
        int size = 0;
        GLenum type;
        glGetActiveAttrib (
            programObject,
            i,
            max_length,
            &actual_length,
            &size,
            &type,
            name
        );

        if (size > 1) {
            for (int j = 0; j < size; j++) {
                std::stringstream ss;
                ss << name << "[" << j << "]";
                std::string long_name = ss.str();
                int location = glGetAttribLocation(programObject, long_name.c_str());
                std::cout << "  " << i << ") type:" << GLTypeToString(type) << " name:" << long_name << " location:" << location << std::endl;
            }
        } else {
            int location = glGetAttribLocation(programObject, name);
            std::cout << "  " << i << ") type:" << GLTypeToString(type) << " name:" << name << " location:" << location << std::endl;
        }
    }

    // Check for uniforms.
    for (int i = 0; i < params; i++) {
        char name[64];
        int max_length = 64;
        int actual_length = 0;
        int size = 0;
        GLenum type;
        glGetActiveAttrib (
            programObject,
            i,
            max_length,
            &actual_length,
            &size,
            &type,
            name
        );

        if (size > 1) {
            for (int j = 0; j < size; j++) {
                std::stringstream ss;
                ss << name << "[" << j << "]";
                std::string long_name = ss.str();
                int location = glGetUniformLocation(programObject, long_name.c_str());
                std::cout << "  " << i << ") type:" << GLTypeToString(type) << " name:" << long_name << " location:" << location << std::endl;
            }
        } else {
            int location = glGetUniformLocation(programObject, name);
            std::cout << "  " << i << ") type:" << GLTypeToString(type) << " name:" << name << " location:" << location << std::endl;
        }
    }
    PrintProgramInfoLog(programObject);
}

/**
 * Transforms an OpenGL type to a usable string.
 * 
 * @param type - the OpenGL type we want to convert.
 * @return - a string representation of an OpenGL type.
 */
const char* Shader::GLTypeToString(GLenum type) {
    switch(type) {
        case GL_BOOL: return "bool";
        case GL_INT: return "int";
        case GL_FLOAT: return "float";
        case GL_FLOAT_VEC2: return "vec2";
        case GL_FLOAT_VEC3: return "vec3";
        case GL_FLOAT_VEC4: return "vec4";
        case GL_FLOAT_MAT2: return "mat2";
        case GL_FLOAT_MAT3: return "mat3";
        case GL_FLOAT_MAT4: return "mat4";
        case GL_SAMPLER_2D: return "sampler2D";
        case GL_SAMPLER_3D: return "sampler3D";
        case GL_SAMPLER_CUBE: return "samplerCube";
        case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
        default: break;
    }
    return "other";
}

void Shader::SetUniformMatrix4fv(const GLchar* name, const GLfloat* value) {
    // GLint location = glGetUniformLocation(m_shaderID, name);

    // glUniformMatrix4fv(location, 1, GL_FALSE, value);

    GLint location = glGetUniformLocation(m_shaderID, name);
    if (location >= 0) {
        glUniformMatrix4fv(location,1,GL_FALSE, value);
    }else {
        std::cout << "Could not find: " << name << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Shader::SetUniform3f(const GLchar* name, float r, float g, float b) {
    GLint location = glGetUniformLocation(m_shaderID, name);

    glUniform3f(location, r, g, b);
}

void Shader::PrintActiveUniforms(GLuint program) {
    GLint numUniforms = 0;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);

    std::vector<GLchar> nameBuffer(256);
    for (GLint i = 0; i < numUniforms; ++i) {
        GLsizei length = 0;
        GLint size = 0;
        GLenum type = 0;
        glGetActiveUniform(program, i, nameBuffer.size(), &length, &size, &type, &nameBuffer[0]);
        std::string name(nameBuffer.begin(), nameBuffer.begin() + length);
        GLint location = glGetUniformLocation(program, name.c_str());

        std::cout << "Uniform " << i << ": " << name << " (type: " << type << ", location: " << location << ")" << std::endl;
    }
}