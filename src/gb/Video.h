#pragma once

#include "Header.h"
#include "Window.h"
#include "Memory.h"
#include "CPU.h"
#include "shader.h"

class Video
{
public:
    Video();
    ~Video();
    void init(Memory *memory);
    void updateGraphics(int cycles, CPU &cpuTemp);
    void setLCDStatus(CPU &cpuTemp);
    bool isLCDEnabled();
    void drawScanLine();
    void renderTiles();
    void renderSprites();
    void render();
    COLOUR getColour(BYTE colourNum, WORD address);

private:
    Memory *_memory;
    int _scanLineCounter;
    GLubyte *_screenDATA;

    // Quad attributes
    enum { VBO_VERTICES, VBO_UVS, VBO_INDICES, VBO_SIZE };

    gfx::Shader m_shader;

    // Opengl handlers
    GLuint m_vao = 0;
    GLuint m_textureId = 0;
    GLuint m_vbos[VBO_SIZE];
};
