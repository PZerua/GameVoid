#pragma once

#include "SDL2\SDL.h"
#include "Header.h"
#include "Memory.h"
#include "Window.h"

class Sprite
{
public:
	Sprite();
	~Sprite();
	void loadSprite(Memory *memory, const WORD &startAddress);
	void render();
	void readLine(Uint32 *pixels, BYTE a, BYTE b, const int &pos);

private:
	BYTE x;
	BYTE y;
	BYTE paletteNum;
	SDL_Texture *_sprite;
	Uint32 *_pixels;
};