#pragma once

#include "Header.h"
#include "SDL2/SDL.h"
#include "Window.h"
#include "Memory.h"
#include "Sprite.h"
#include "CPU.h"

class Video
{
public:
	Video();
	~Video();
	void init(Memory *memory);
	void loadSprite(Memory *memory);
	void renderSprite();
	void updateGraphics(const int &cycles, CPU &cpuTemp);
	void setLCDStatus(CPU &cpuTemp);
	bool isLCDEnabled();
	void drawScanLine();
	void renderTiles();
	void renderSprites();
	void render();
	COLOUR getColour(BYTE colourNum, WORD address);
	Window _window;

private:
	Memory *_memory;
	Sprite test;
	int _scanLineCounter;
	Uint32 *_screenDATA;
	SDL_Texture *BG_WD;
};
