#pragma once

#include "data_types.h"
#include "graphics/shader.h"
#include "graphics/window.h"
#include "cpu.h"

class Memory;

class Video
{
public:

    enum COLOUR {
        WHITE,
        LIGHT_GREY,
        DARK_GREY,
        BLACK
    };

    Video();
    ~Video();
    void init(Memory *memory);
    void updateGraphics(int cycles, CPU &cpuTemp);
    void setLCDStatus(CPU &cpuTemp);
    bool isLCDEnabled();
    void drawScanLine();
    void render();
    COLOUR getColor(BYTE colourNum, WORD address);

    struct Sprite {
        BYTE y;
        BYTE x;
        BYTE tileIndex;
        BYTE flags;
    };

private:

    BYTE getTileIndex(BYTE x, BYTE y, bool tileMap);
    BYTE getTileColor(BYTE x, BYTE y, BYTE tileIndex, bool addressingMode);

    void setPixel(BYTE x, BYTE y, COLOUR col);

    std::vector<Sprite> getSprites(BYTE y, WORD spriteHeight);

    Memory*  memory = nullptr;
    int      m_scanLineCounter = 456;
    GLubyte* m_screenDATA = nullptr;

    bool LYC_interrupt_served = false;
    int window_y_counter = 0;

    // Quad attributes
    enum { VBO_VERTICES, VBO_UVS, VBO_INDICES, VBO_SIZE };

    gfx::Shader m_shader;

    // Opengl handlers
    GLuint m_vao = 0;
    GLuint m_textureId = 0;
    GLuint m_vbos[VBO_SIZE] = {};
};
