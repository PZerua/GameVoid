#include "Video.h"

#include "io_registers.h"
#include "interrupts.h"
#include "utils/bit_utils.h"
#include "utils/debug_utils.h"
#include "memory.h"

#include <cassert>
#include <algorithm>

using namespace utils;

const char* vertexShader =  R"(#version 330 core

layout(location = 0) in vec2 vertex;
layout(location = 1) in vec2 uv;

out vec2 uvs;

void main()
{
    uvs = uv;

    // Output position of the vertex
    gl_Position = vec4(vertex, 0.0f, 1.0f);
}

)";

const char* fragmentShader = R"(#version 330 core

in vec2 uvs;
out vec4 fragColor;

uniform sampler2D fboTex;

void main(void)
{
	vec2 uvFlip = uvs;
	uvFlip.y = 1.0 - uvFlip.y;
    vec4 fbo = texture(fboTex, uvFlip);
    fragColor = fbo;
}

)";

Video::Video()
{
    m_screenDATA = new GLubyte[160 * 144 * 3];
    memset(m_screenDATA, 255, 160 * 144 * 3 * sizeof(GLubyte));
}

Video::~Video()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(VBO_SIZE, m_vbos);
    glDeleteTextures(1, &m_textureId);

    delete[] m_screenDATA;
}

void Video::init(Memory *memory)
{
    this->memory = memory;

    m_shader.init(vertexShader, fragmentShader);

    // Generate texture
    glGenTextures(1, &m_textureId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    // Create buffer for the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<int>(160), static_cast<int>(144), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_screenDATA);

    // set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // defining the data to draw
    GLfloat vertices[] = {
       -1.0f,  1.0f,  // top left
       -1.0f, -1.0f,  // bottom left
        1.0f, -1.0f,  // bottom right
        1.0f,  1.0f   // top right
    };

    GLfloat uvs[] = {
        0.0f, 1.0f,   // top left
        0.0f, 0.0f,   // bottom left
        1.0f, 0.0f,   // bottom right
        1.0f, 1.0f    // top right
    };

    GLushort indices[] = { 0, 1, 2, 3, 0, 2 };

    // Create vertex array object
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // Create vertex buffer objects for vertices, uvs and indices
    glGenBuffers(3, m_vbos);

    // Setup vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[VBO_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(VBO_VERTICES, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(VBO_VERTICES);

    // Setup UVs
    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[VBO_UVS]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(VBO_UVS, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(VBO_UVS);

    // Setup indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbos[VBO_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Video::updateGraphics(int cycles, CPU &cpu)
{
    setLCDStatus(cpu);

    if (isLCDEnabled())
        m_scanLineCounter -= cycles;
    else
        return;

    if (m_scanLineCounter <= 0)
    {
        LYC_interrupt_served = false;
        window_y_counter = 0;

        BYTE currentline = memory->read(LY);

        m_scanLineCounter += 456;

        if (currentline <= 153) {
            // we have entered vertical blank period
            if (currentline == 144)
                cpu.requestInterrupt(VBlank);

            // draw the current scanline 
            else if (currentline < 144)
                drawScanLine();

            // time to move onto next scanline
            memory->directModification(LY, currentline + 1);
        }
        else {
            // if gone past scanline 153 reset to 0
            memory->directModification(LY, 0x00);
        }
    }
}

void Video::setLCDStatus(CPU &cpuTemp)
{
    BYTE status = memory->read(STAT);
    if (!isLCDEnabled())
    {
        // set the mode to 1 during lcd disabled and reset scanline
        m_scanLineCounter = 456;
        memory->directModification(LY, 0x00);
        status &= 0xFC;
        status = bitSet(status, 0);
        memory->write(STAT, status);
        return;
    }

    BYTE currentline = memory->read(LY);
    BYTE currentmode = status & 0x03;

    BYTE mode = 0x00;
    bool reqInt = false;

    // If greater we are in VBLank period
    if (currentline >= 144)
    {
        mode = 0x01;
        status = bitSet(status, 0);
        status = bitReset(status, 1);
        reqInt = testBit(status, 4);
    }
    // Else, we are drawing scanlines in panel
    else
    {
        int mode2bounds = 456 - 80;
        int mode3bounds = mode2bounds - 172;

        // mode 2
        if (m_scanLineCounter >= mode2bounds)
        {
            mode = 0x02;
            status = bitSet(status, 1);
            status = bitReset(status, 0);
            reqInt = testBit(status, 5);
        }
        // mode 3
        else if (m_scanLineCounter >= mode3bounds)
        {
            mode = 0x03;
            status = bitSet(status, 1);
            status = bitSet(status, 0);
        }
        // mode 0
        else
        {
            mode = 0x00;
            status = bitReset(status, 1);
            status = bitReset(status, 0);
            reqInt = testBit(status, 3);
        }
    }

    // just entered a new mode so request interrupt
    if (reqInt && (mode != currentmode))
        cpuTemp.requestInterrupt(LCD);

    // check the conincidence flag
    if (memory->read(LY) == memory->read(LYC))
    {
        status = bitSet(status, 2);
        if (testBit(status, 6) && !LYC_interrupt_served) {
            cpuTemp.requestInterrupt(LCD);
            LYC_interrupt_served = true;
        }
    }
    else
    {
        status = bitReset(status, 2);
    }
    memory->write(STAT, status);
}

bool Video::isLCDEnabled()
{
    return bitIntersect(memory->read(LCDC), eLCDControl::ENABLE);
}

void Video::drawScanLine()
{
    // where to draw the visual area and the window
    BYTE ly = memory->read(LY);
    WORD scrollX = memory->read(SCX);
    WORD scrollY = memory->read(SCY);
    WORD windowY = memory->read(WY);
    WORD windowX = std::clamp((int)memory->read(WX) - 7, 0, 255);

    BYTE LCDC_flags = memory->read(LCDC);

    int spriteHeight = 8;

    if (bitIntersect(memory->read(LCDC), eLCDControl::OBJ_SIZE))
        spriteHeight = 16;

    std::vector<Sprite> sprites = getSprites(memory->read(LY), spriteHeight);

    for (int x = 0; x < 160; x++)
    {
        int tileX = 0;
        int tileY = 0;

        const Sprite* drawnSprite = nullptr;

        COLOUR spriteColor;

        // OBJ Enabled
        if (bitIntersect(LCDC_flags, eLCDControl::OBJ_ENABLE)) {

            BYTE objX = x + 8;
            for (const Video::Sprite& sprite : sprites)
            {
                if (objX < std::clamp(WORD(sprite.x) + 8, 0, 255) && objX >= sprite.x) {
                    bool yFlip = bitIntersect(sprite.flags, eSpriteFlags::Y_FLIP);
                    bool xFlip = bitIntersect(sprite.flags, eSpriteFlags::X_FLIP);

                    tileX = x - (int(sprite.x) - 8);
                    tileX %= 8;

                    // read the sprite in backwards in the y axis
                    if (xFlip)
                    {
                        tileX = 7 - tileX;
                    }

                    int scanline = memory->read(LY);

                    tileY = scanline - (int(sprite.y) - 16);
                    tileY %= spriteHeight;

                    // read the sprite in backwards in the y axis
                    if (yFlip)
                    {
                        tileY = spriteHeight - 1 - tileY;
                    }

                    BYTE tileIndex = sprite.tileIndex;
                    if (bitIntersect(memory->read(LCDC), eLCDControl::OBJ_SIZE)) {
                        tileIndex &= 0b1111'1110;
                        if (tileY >= 8) {
                            tileY -= 8;
                            tileIndex += 1;
                        }
                    }

                    BYTE colId = getTileColor(tileX, tileY, tileIndex, false);

                    // not transparent
                    if (colId != 0) {
                        drawnSprite = &sprite;
                        WORD palette = bitIntersect(sprite.flags, eSpriteFlags::PALETTE) ? 0xFF49 : 0xFF48;
                        spriteColor = getColor(colId, palette);
                        break;
                    }
                }
            }
        }

        bool spriteOnBackground = false;
        if (drawnSprite) {
            if (!bitIntersect(drawnSprite->flags, eSpriteFlags::PRIORITY)) {
                setPixel(x, ly, spriteColor);
                continue;
            }

            spriteOnBackground = true;
        }

        if (!bitIntersect(LCDC_flags, eLCDControl::BG_WINDOW_ENABLE)) {
            if (spriteOnBackground) {
                setPixel(x, ly, spriteColor);
            }
            else {
                COLOUR col = getColor(0, BGP);
                setPixel(x, ly, col);
            }

            continue;
        }

        bool usingWindow = bitIntersect(LCDC_flags, eLCDControl::WINDOW_ENABLE) && x >= windowX && ly >= windowY;

        if (!usingWindow)
        {
            tileX = WORD(x + scrollX) % 256;
            tileY = WORD(ly + scrollY) % 256;
        } else {
            tileX = (x - windowX);
            tileY = window_y_counter;
        }

        WORD tileIndex = getTileIndex(tileX, tileY, bitIntersect(memory->read(LCDC), usingWindow ? eLCDControl::WINDOW_TILE_MAP : eLCDControl::BG_TILE_MAP));

        BYTE colourNum = getTileColor(tileX, tileY, tileIndex, !bitIntersect(memory->read(LCDC), eLCDControl::TILE_DATA_AREA));

        if (colourNum == 0 && spriteOnBackground)
        {
            setPixel(x, ly, spriteColor);
        }
        else {
            COLOUR col = getColor(colourNum, BGP);
            setPixel(x, ly, col);
        }
    }

    if (bitIntersect(LCDC_flags, eLCDControl::WINDOW_ENABLE) && windowX < 160 && windowY <= ly) {
        window_y_counter++;
    }
}

Video::COLOUR Video::getColor(BYTE colourNum, WORD address)
{
    COLOUR res = WHITE;
    BYTE palette = memory->read(address);
    int hi = 0;
    int lo = 0;

    // which bits of the colour palette does the colour id map to?
    switch (colourNum)
    {
    case 0: hi = 1; lo = 0; break;
    case 1: hi = 3; lo = 2; break;
    case 2: hi = 5; lo = 4; break;
    case 3: hi = 7; lo = 6; break;
    }

    // use the palette to get the colour
    int colour = 0;
    colour = bitGetVal(palette, hi) << 1;
    colour |= bitGetVal(palette, lo);

    // convert the game colour to emulator colour
    switch (colour)
    {
    case 0: res = WHITE; break;
    case 1: res = LIGHT_GREY; break;
    case 2: res = DARK_GREY; break;
    case 3: res = BLACK; break;
    }

    return res;
}

BYTE Video::getTileIndex(BYTE x, BYTE y, bool tileMap)
{
    WORD tileMapIndex = ((WORD(y) / 8) * 32) + WORD(x) / 8;
    WORD tileRoot = tileMap ? 0x1C00 : 0x1800;

    return memory->read(0x8000 + tileRoot + tileMapIndex);
}

BYTE Video::getTileColor(BYTE x, BYTE y, BYTE tileIndex, bool addressingMode)
{
    WORD byteIndex = (16 * WORD(tileIndex)) + (2 * (WORD(y) % 8));

    if (addressingMode && tileIndex < 128) {
        byteIndex += 0x1000;
    }

    BYTE data1 = memory->read(0x8000 + byteIndex);
    BYTE data2 = memory->read(0x8000 + byteIndex + 1);

    BYTE a = (data1 & (0b10000000 >> (x % 8))) != 0;
    BYTE b = (data2 & (0b10000000 >> (x % 8))) != 0;

    // Get color ID from the two bits
    return a | (b << 1);
}

void Video::setPixel(BYTE x, BYTE y, COLOUR col)
{
    int red = 0x08;
    int green = 0x18;
    int blue = 0x20;

    // setup the RGB values
    switch (col)
    {
    case WHITE:         red = 0xE0; green = 0xF8; blue = 0xD0; break;
    case LIGHT_GREY:    red = 0x88; green = 0xC0; blue = 0x70; break;
    case DARK_GREY:     red = 0x34; green = 0x68; blue = 0x56; break;
    }

    // safety check to make sure what im about 
    // to set is in the 160x144 bounds
    if ((y < 0) || (y > 143) || (x < 0) || (x > 159))
    {
        assert(0);
    }

    m_screenDATA[y * 160 * 3 + (x * 3)] = red;
    m_screenDATA[y * 160 * 3 + (x * 3) + 1] = green;
    m_screenDATA[y * 160 * 3 + (x * 3) + 2] = blue;
}

bool compareByX(const Video::Sprite& a, const Video::Sprite& b)
{
    return a.x < b.x;
}

std::vector<Video::Sprite> Video::getSprites(BYTE y, WORD spriteHeight)
{
    std::vector<Sprite> sprites;
    BYTE objY = y + 16;

    for (int spriteIdx = 0; spriteIdx < 40; spriteIdx++)
    {
        Sprite sprite;
        // sprite occupies 4 bytes in the sprite attributes table
        BYTE index = spriteIdx * 4;
        sprite.y = memory->read(0xFE00 + index);
        sprite.x = memory->read(0xFE00 + index + 1);
        sprite.tileIndex = memory->read(0xFE00 + index + 2);
        sprite.flags = memory->read(0xFE00 + index + 3);

        if (objY < (sprite.y + spriteHeight) && objY >= sprite.y) {
            sprites.push_back(sprite);

            if (sprites.size() == 10)
                break;
        }
    }

    std::sort(sprites.begin(), sprites.end(), compareByX);

    return sprites;
}

void Video::render()
{
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    // Update fbo
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, static_cast<int>(160), static_cast<int>(144), GL_RGB, GL_UNSIGNED_BYTE, m_screenDATA);

    // Bind Shader
    m_shader.enable();

    // Bind vertex array object
    glBindVertexArray(m_vao);

    // Draw
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

    glBindVertexArray(0);

    m_shader.disable();
}
