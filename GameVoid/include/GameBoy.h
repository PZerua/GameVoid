#pragma once

#include "Header.h"
#include "Cartridge.h"
#include "Memory.h"
#include "CPU.h"
#include "Video.h"
#include "Controller.h"

#define MAXCYCLES 69905

class GameBoy
{
public:
	GameBoy();
	~GameBoy();
	void update();

private:
	Cartridge _game;
	Memory _memory;
	Video _video;
	CPU _CPU;
	Controller _controller;
};
