#include "Video.h"

Video::Video()
{
	_window.init("GameVoid", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

	_backgroundBuffer = SDL_CreateTexture(Window::mRenderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
	_pixels = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
	memset(_pixels, 255, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
}

Video::~Video()
{

}

void Video::loadSprite(Memory *memory)
{
	test.loadSprite(memory, 0xFE00);
}

void Video::renderSprite()
{
	_window.Clear();
	test.render();
	_window.Present();
}
