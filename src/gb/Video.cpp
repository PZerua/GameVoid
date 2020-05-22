#include "Video.h"

#include "bitUtils.h"
#include "debugutils.h"

Video::Video()
{
    _scanLineCounter = 456;
    _screenDATA = new GLubyte[160 * 144 * 3];
    memset(_screenDATA, 255, 160 * 144 * 3 * sizeof(GLubyte));
}

Video::~Video()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(VBO_SIZE, m_vbos);
    glDeleteTextures(1, &m_textureId);

    delete[] _screenDATA;
}

void Video::init(Memory *memory)
{
    _memory = memory;

    m_shader.init("quad");

    // Generate texture
    glGenTextures(1, &m_textureId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    // Create buffer for the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<int>(160), static_cast<int>(144), 0, GL_RGBA, GL_UNSIGNED_BYTE, _screenDATA);

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

void Video::updateGraphics(int cycles, CPU &cpuTemp)
{
    setLCDStatus(cpuTemp);

    if (isLCDEnabled())
        _scanLineCounter -= cycles;
    else
        return;

    if (_scanLineCounter <= 0)
    {
        // time to move onto next scanline
        _memory->directModification(LY, _memory->read(LY) + 1);
        BYTE currentline = _memory->read(LY);

        _scanLineCounter = 456;

        // we have entered vertical blank period
        if (currentline == 144)
            cpuTemp.requestInterrupt(VBlank);

        // if gone past scanline 153 reset to 0
        else if (currentline > 153)
            _memory->directModification(LY, 0x00);

        // draw the current scanline 
        else if (currentline < 144)
            drawScanLine();
    }
}

void Video::setLCDStatus(CPU &cpuTemp)
{
    BYTE status = _memory->read(0xFF41);
    if (!isLCDEnabled())
    {
        // set the mode to 1 during lcd disabled and reset scanline
        _scanLineCounter = 456;
        _memory->directModification(LY, 0x00);
        status &= 0xFC;
        status = bitSet(status, 0);
        _memory->write(0xFF41, status);
        return;
    }

    BYTE currentline = _memory->read(LY);
    BYTE currentmode = status & 0x03;

    BYTE mode = 0x00;
    bool reqInt = false;

    // If is greater we are in VBLank period
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
        if (_scanLineCounter >= mode2bounds)
        {
            mode = 0x02;
            status = bitSet(status, 1);
            status = bitReset(status, 0);
            reqInt = testBit(status, 5);
        }
        // mode 3
        else if (_scanLineCounter >= mode3bounds)
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

    // just entered a new mode so request interupt
    if (reqInt && (mode != currentmode))
        cpuTemp.requestInterrupt(LCD);

    // check the conincidence flag
    if (_memory->read(LY) == _memory->read(LYC))
    {
        status = bitSet(status, 2);
        if (testBit(status, 6))
            cpuTemp.requestInterrupt(LCD);
    }
    else
    {
        status = bitReset(status, 2);
    }
    _memory->write(0xFF41, status);
}

bool Video::isLCDEnabled()
{
    return ((_memory->read(LCDC) & 0x80) == 0x80);
}

void Video::drawScanLine()
{
    BYTE control = _memory->read(LCDC);
    if (testBit(control, 0))
        renderTiles();

    if (testBit(control, 1))
        renderSprites();
}

void Video::renderTiles()
{
    WORD tileData = 0;
    WORD backgroundMemory = 0;
    bool unsig = true;

    // where to draw the visual area and the window
    BYTE scrollY = _memory->read(SCY);
    BYTE scrollX = _memory->read(SCX);
    BYTE windowY = _memory->read(WY);
    BYTE windowX = _memory->read(WX) - 7;

    bool usingWindow = false;

    // is the window enabled?
    if (testBit(_memory->read(LCDC), 5))
    {
        // is the current scanline we're drawing 
        // within the windows Y pos?,
        if (windowY <= _memory->read(LY))
            usingWindow = true;
    }

    // which tile data are we using? 
    if (testBit(_memory->read(LCDC), 4))
    {
        tileData = 0x8000;
    }
    else
    {
        // IMPORTANT: This memory region uses signed 
        // bytes as tile identifiers
        tileData = 0x8800;
        unsig = false;
    }

    // which background mem?
    if (false == usingWindow)
    {
        if (testBit(_memory->read(LCDC), 3))
            backgroundMemory = 0x9C00;
        else
            backgroundMemory = 0x9800;
    }
    else
    {
        // which window memory?
        if (testBit(_memory->read(LCDC), 6))
            backgroundMemory = 0x9C00;
        else
            backgroundMemory = 0x9800;
    }

    BYTE yPos = 0;

    // yPos is used to calculate which of 32 vertical tiles the 
    // current scanline is drawing
    if (!usingWindow)
        yPos = scrollY + _memory->read(LY);
    else
        yPos = _memory->read(LY) - windowY;

    // which of the 8 vertical pixels of the current 
    // tile is the scanline on?
    WORD tileRow = (((BYTE)(yPos / 8)) * 32);

    // time to start drawing the 160 horizontal pixels
    // for this scanline
    for (int pixel = 0; pixel < 160; pixel++)
    {
        BYTE xPos = pixel + scrollX;

        // translate the current x pos to window space if necessary
        if (usingWindow)
        {
            if (pixel >= windowX)
            {
                xPos = pixel - windowX;
            }
        }

        // which of the 32 horizontal tiles does this xPos fall within?
        WORD tileCol = (xPos / 8);
        SIGNED_WORD tileNum;

        // get the tile identity number. Remember it can be signed
        // or unsigned
        WORD tileAddrss = backgroundMemory + tileRow + tileCol;
        if (unsig)
            tileNum = (BYTE)_memory->read(tileAddrss);
        else
            tileNum = (SIGNED_BYTE)_memory->read(tileAddrss);

        // deduce where this tile identifier is in memory. Remember i 
        // shown this algorithm earlier
        WORD tileLocation = tileData;

        if (unsig)
            tileLocation += (tileNum * 16);
        else
            tileLocation += ((tileNum + 128) * 16);

        // find the correct vertical line we're on of the 
        // tile to get the tile data 
        // from in memory
        BYTE line = yPos % 8;
        line *= 2; // each vertical line takes up two bytes of memory
        BYTE data1 = _memory->read(tileLocation + line);
        BYTE data2 = _memory->read(tileLocation + line + 1);

        // pixel 0 in the tile is it 7 of data 1 and data2.
        // Pixel 1 is bit 6 etc..
        int colourBit = xPos % 8;
        colourBit -= 7;
        colourBit *= -1;

        // combine data 2 and data 1 to get the colour id for this pixel 
        // in the tile
        int colourNum = bitGetVal(data2, colourBit);
        colourNum <<= 1;
        colourNum |= bitGetVal(data1, colourBit);

        // now we have the colour id get the actual 
        // colour from palette 0xFF47
        COLOUR col = getColor(colourNum, BGP);
        int red = 0;
        int green = 0;
        int blue = 0;

        // setup the RGB values
        switch (col)
        {
        case WHITE:    red = 255; green = 255; blue = 255; break;
        case LIGHT_GREY:red = 0xCC; green = 0xCC; blue = 0xCC; break;
        case DARK_GREY:    red = 0x77; green = 0x77; blue = 0x77; break;
        }

        int finaly = _memory->read(0xFF44);

        // safety check to make sure what im about 
        // to set is in the 160x144 bounds
        if ((finaly<0) || (finaly>143) || (pixel<0) || (pixel>159))
        {
            continue;
        }

        _screenDATA[finaly * 160 * 3 + (pixel * 3)] = red;
        _screenDATA[finaly * 160 * 3 + (pixel * 3) + 1] = green;
        _screenDATA[finaly * 160 * 3 + (pixel * 3) + 2] = blue;
    }
}

COLOUR Video::getColor(BYTE colourNum, WORD address)
{
    COLOUR res = WHITE;
    BYTE palette = _memory->read(address);
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

void Video::renderSprites()
{
    bool use8x16 = false;
    if (testBit(_memory->read(LCDC), 2))
        use8x16 = true;

    for (int sprite = 0; sprite < 40; sprite++)
    {
        // sprite occupies 4 bytes in the sprite attributes table
        BYTE index = sprite * 4;
        BYTE yPos = _memory->read(0xFE00 + index) - 16;
        BYTE xPos = _memory->read(0xFE00 + index + 1) - 8;
        BYTE tileLocation = _memory->read(0xFE00 + index + 2);
        BYTE attributes = _memory->read(0xFE00 + index + 3);

        bool yFlip = testBit(attributes, 6);
        bool xFlip = testBit(attributes, 5);

        int scanline = _memory->read(LY);

        int ysize = 8;
        if (use8x16)
            ysize = 16;

        // does this sprite intercept with the scanline?
        if ((scanline >= yPos) && (scanline < (yPos + ysize)))
        {
            int line = scanline - yPos;

            // read the sprite in backwards in the y axis
            if (yFlip)
            {
                line -= ysize;
                line *= -1;
            }

            line *= 2; // same as for tiles
            WORD dataAddress = (0x8000 + (tileLocation * 16)) + line;
            BYTE data1 = _memory->read(dataAddress);
            BYTE data2 = _memory->read(dataAddress + 1);

            // its easier to read in from right to left as pixel 0 is 
            // bit 7 in the colour data, pixel 1 is bit 6 etc...
            for (int tilePixel = 7; tilePixel >= 0; tilePixel--)
            {
                int colourbit = tilePixel;
                // read the sprite in backwards for the x axis 
                if (xFlip)
                {
                    colourbit -= 7;
                    colourbit *= -1;
                }

                // the rest is the same as for tiles
                int colourNum = bitGetVal(data2, colourbit);
                colourNum <<= 1;
                colourNum |= bitGetVal(data1, colourbit);

                WORD colourAddress = testBit(attributes, 4) ? 0xFF49 : 0xFF48;
                COLOUR col = getColor(colourNum, colourAddress);

                // white is transparent for sprites.
                if (col == WHITE)
                    continue;

                int red = 0;
                int green = 0;
                int blue = 0;

                switch (col)
                {
                case WHITE: red = 255; green = 255; blue = 255; break;
                case LIGHT_GREY:red = 0xCC; green = 0xCC; blue = 0xCC; break;
                case DARK_GREY:red = 0x77; green = 0x77; blue = 0x77; break;
                }

                int xPix = 0 - tilePixel;
                xPix += 7;

                int pixel = xPos + xPix;

                // sanity check
                if ((scanline<0) || (scanline>143) || (pixel<0) || (pixel>159))
                {
                    continue;
                }

                // check if pixel is hidden behind background
                if (testBit(attributes, 7) == 1)
                {
                    if (((_screenDATA[scanline * 160 + pixel]) != 0xFF) || ((_screenDATA[scanline * 160 + pixel + 1] & 0xFF) != 0xFF) || ((_screenDATA[scanline * 160 + pixel + 2] & 0xFF) != 0xFF))
                        continue;
                }

                _screenDATA[scanline * 160 * 3 + (pixel * 3)] = red;
                _screenDATA[scanline * 160 * 3 + (pixel * 3) + 1] = green;
                _screenDATA[scanline * 160 * 3 + (pixel * 3) + 2] = blue;
            }
        }
    }
}

void Video::render()
{
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    // Update fbo
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, static_cast<int>(160), static_cast<int>(144), GL_RGB, GL_UNSIGNED_BYTE, _screenDATA);

    // Bind Shader
    m_shader.enable();

    // Bind vertex array object
    glBindVertexArray(m_vao);

    // Draw
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

    glBindVertexArray(0);

    m_shader.disable();
}
