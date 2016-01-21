#include "Sprite.h"

Sprite::Sprite()
{
	
}

Sprite::~Sprite()
{
	delete[] _pixels;
}

void Sprite::loadSprite(Memory *memory, const WORD &startAddress)
{
	BYTE y = memory->read(0xFE00);
	BYTE x = memory->read(0xFE01);
	BYTE num = memory->read(0xFE02);

	_sprite = SDL_CreateTexture(Window::mRenderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STATIC, 8, 8);
	_pixels = new Uint32[8 * 8];
	memset(_pixels, 0, 8 * 8 * sizeof(Uint32));

	for (int i = 0; i < 64; i++)
	{
		readLine(_pixels, memory->read(0x8180), memory->read(0x8181 + i * 2), i);
	}
}

void Sprite::render()
{
	SDL_UpdateTexture(_sprite, NULL, _pixels, 8 * sizeof(Uint32));
	SDL_RenderCopy(Window::mRenderer, _sprite, NULL, NULL);
}

void Sprite::readLine(Uint32 *pixels, BYTE a, BYTE b, const int &pos)
{
	for (int i = 0; i < 8; i++)
	{
		BYTE tmp = ((a & (1 << i)) >> i) + (((b & (1 << i)) >> i) << 1);
		
		if (tmp == 0x00)
			_pixels[pos] = COLOR_GREEN_0;
		else if (tmp = 0x01)
			_pixels[pos] = COLOR_GREEN_1;
		else if (tmp = 0x02)
			_pixels[pos] = COLOR_GREEN_2;
		else if (tmp = 0x03)
			_pixels[pos] = COLOR_GREEN_3;
	}
}