#pragma once

#include "Header.h"
#include "SDL2/SDL.h"
#include "Window.h"
#include "Memory.h"
#include "Sprite.h"

class Video
{
public:
	Video();
	~Video();
	void loadSprite(Memory *memory);
	void renderSprite();
	Window _window;

private:
	
	Sprite test;
	SDL_Texture *_backgroundBuffer;
	Uint32 *_pixels;
};
