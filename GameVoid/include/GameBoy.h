#pragma once

#include "Header.h"
#include "Cartridge.h"
#include "Memory.h"
#include "CPU.h"
#include "Video.h"

class GameBoy
{
public:
	GameBoy();
	~GameBoy();
	void init();

private:
	Cartridge _game;
	Memory _memory;
	Video _video;
	CPU _CPU;

};
