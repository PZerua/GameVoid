#pragma once
#include "Header.h"
#include "SDL2/SDL.h"

class Controller
{
public:
	Controller();
	~Controller();
	void init(BYTE *mem);
	BYTE getJoypadState() const;
	void checkControls(SDL_Event *event);
	void keyPressed(const int &key);
	bool requestInterrupt();
	void setRequestInterrupt(const bool &state);
private:
	BYTE _joypadState;
	BYTE *memory;
	bool _requestInterrupt;
};