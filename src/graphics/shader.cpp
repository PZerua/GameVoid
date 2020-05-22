/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under MIT license. Read LICENSE for more details.
*/

#include "shader.h"

#include <iostream>

namespace gfx
{

Shader::~Shader()
{
    glDeleteProgram(m_programId);
}

void Shader::init(const char* vertexShader, const char* fragmentShader)
{
    // Compile shaders
    GLuint vertex, fragment;

    compileShader(vertex, GL_VERTEX_SHADER, vertexShader);
    compileShader(fragment, GL_FRAGMENT_SHADER, fragmentShader);

    // Shader Program
    m_programId = glCreateProgram();
    glAttachShader(m_programId, vertex);
    glAttachShader(m_programId, fragment);
    glLinkProgram(m_programId);

    // Print errors if any
    printProgramErrors(m_programId);

    // Delete unnecessary shaders (we have program)
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::compileShader(GLuint &shader, GLenum shaderType, const char *shaderCode)
{
    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    // Print compile errors if any
    printShaderErrors(shader);
}

void Shader::printShaderErrors(GLuint shader) const
{
    GLint success;
    char compileLog[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, compileLog);
        std::cout << "Error in shader compilation\n" << compileLog << std::endl;
    }
}

void Shader::printProgramErrors(GLuint shader) const
{
    GLint success;
    char linkLog[512];

    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader, 512, NULL, linkLog);
        std::cout << "Error in shader compilation\n" << linkLog << std::endl;
    }
}

void Shader::enable()
{
    glUseProgram(m_programId);
}

void Shader::disable()
{
    glUseProgram(0);
}

void Shader::enable(GLuint programId)
{
    glUseProgram(programId);
}

void Shader::disable(GLuint programId)
{
    glUseProgram(0);
}

void Shader::setBool(GLuint programId, const char* name, bool value)
{
    glUniform1i(glGetUniformLocation(programId, name), (int)value);
}
void Shader::setInt(GLuint programId, const char* name, int value)
{
    glUniform1i(glGetUniformLocation(programId, name), value);
}
void Shader::setFloat(GLuint programId, const char* name, float value)
{
    glUniform1f(glGetUniformLocation(programId, name), value);
}

} // namespace gfx
