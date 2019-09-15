/**
* (c) 2017 Pablo Luis Garc�a. All rights reserved.
* Released under MIT license. Read LICENSE for more details.
*/

#pragma once

#include "../utils/fileutils.h"
#include <GL/glew.h>

namespace gfx
{

class Shader
{
private:
    // Program ID
    GLuint m_programId;

public:
    // Builds the shader
    Shader() = default;
    Shader(const std::string &shaderName);
    ~Shader();

    void init(const std::string &shaderName);

    // Get program ID
    inline GLuint getId() const { return m_programId; }

    // Activate the shader
    void enable();
    void disable();
    static void enable(GLuint programId);
    static void disable(GLuint programId);
    // Utility uniform functions
    static void setBool(GLuint programId, const char* name, bool value);
    static void setInt(GLuint programId, const char* name, int value);
    static void setFloat(GLuint programId, const char* name, float value);

private:
    // Utility functions
    void compileShader(GLuint &shader, GLenum shaderType, const char *shaderCode);
    void printShaderErrors(GLuint shader) const;
    void printProgramErrors(GLuint shader) const;
};

} // namespace gfx