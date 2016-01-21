#include "Video.h"

Video::Video()
{
	_window.init("GameVoid", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	_scanLineCounter = 456;
	BG_WD = SDL_CreateTexture(Window::mRenderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STATIC, 160, 144);
}

Video::~Video()
{

}

void Video::init(Memory *memory)
{
	_memory = memory;
}

void Video::updateGraphics(const int &cycles, CPU &cpuTemp)
{
	setLCDStatus(cpuTemp);

	if (isLCDEnabled())
		_scanLineCounter -= cycles;
	else
		return;

	if (_scanLineCounter <= 0)
	{
		// time to move onto next scanline
		_memory->directModification(0xFF44, _memory->read(0xFF44) + 1);
		BYTE currentline = _memory->read(0xFF44);

		_scanLineCounter = 456;

		// we have entered vertical blank period
		if (currentline == 144)
			cpuTemp.requestInterrupt(VBlank);

		// if gone past scanline 153 reset to 0
		else if (currentline > 153)
			_memory->directModification(0xFF44, 0x00);

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
		_memory->directModification(0xFF44, 0x00);
		status &= 0xFC;
		status = bitSet(status, 0);
		_memory->write(0xFF41, status);
		return;
	}

	BYTE currentline = _memory->read(0xFF44);
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
		cpuTemp.requestInterrupt(VBlank);

	// check the conincidence flag
	if (_memory->read(0xFF44) == _memory->read(0xFF45))
	{
		status = bitSet(status, 2);
		if (testBit(status, 6))
			cpuTemp.requestInterrupt(VBlank);
	}
	else
	{
		status = bitReset(status, 2);
	}
	_memory->write(0xFF41, status);
}

bool Video::isLCDEnabled()
{
	return ((_memory->read(0xFF40) & 0x80) == 0x80);
}

void Video::drawScanLine()
{
	BYTE control = _memory->read(0xFF40);
	if (testBit(control, 0))
		renderTiles();

	/*if (testBit(control, 1))
		renderSprites();*/
}

void Video::renderTiles()
{
	WORD tileData = 0;
	WORD backgroundMemory = 0;
	bool unsig = true;

	// where to draw the visual area and the window
	BYTE scrollY = _memory->read(0xFF42);
	BYTE scrollX = _memory->read(0xFF43);
	BYTE windowY = _memory->read(0xFF4A);
	BYTE windowX = _memory->read(0xFF4B) - 7;

	bool usingWindow = false;

	// is the window enabled?
	if (testBit(_memory->read(0xFF40), 5))
	{
		// is the current scanline we're drawing 
		// within the windows Y pos?,
		if (windowY <= _memory->read(0xFF44))
			usingWindow = true;
	}

	// which tile data are we using? 
	if (testBit(_memory->read(0xFF40), 4))
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
		if (testBit(_memory->read(0xFF40), 3))
			backgroundMemory = 0x9C00;
		else
			backgroundMemory = 0x9800;
	}
	else
	{
		// which window memory?
		if (testBit(_memory->read(0xFF40), 6))
			backgroundMemory = 0x9C00;
		else
			backgroundMemory = 0x9800;
	}

	BYTE yPos = 0;

	// yPos is used to calculate which of 32 vertical tiles the 
	// current scanline is drawing
	if (!usingWindow)
		yPos = scrollY + _memory->read(0xFF44);
	else
		yPos = _memory->read(0xFF44) - windowY;

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
		//from in memory
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
		COLOUR col = getColour(colourNum, 0xFF47);
		int red = 0;
		int green = 0;
		int blue = 0;

		// setup the RGB values
		switch (col)
		{
		case WHITE:	red = 255; green = 255; blue = 255; break;
		case LIGHT_GREY:red = 0xCC; green = 0xCC; blue = 0xCC; break;
		case DARK_GREY:	red = 0x77; green = 0x77; blue = 0x77; break;
		}

		int finaly = _memory->read(0xFF44);

		// safety check to make sure what im about 
		// to set is int the 160x144 bounds
		if ((finaly<0) || (finaly>143) || (pixel<0) || (pixel>159))
		{
			continue;
		}

		_screenDATA[pixel][finaly][0] = red;
		_screenDATA[pixel][finaly][1] = green;
		_screenDATA[pixel][finaly][2] = blue;
	}
}

COLOUR Video::getColour(BYTE colourNum, WORD address)
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

void Video::render()
{
	SDL_RenderClear(Window::mRenderer);

	SDL_UpdateTexture(BG_WD, NULL, _pixels, 144 * sizeof(Uint32));
	SDL_RenderCopy(Window::mRenderer, BG_WD, NULL, NULL);

	SDL_RenderPresent(Window::mRenderer);
}