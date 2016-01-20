#include "GameBoy.h"

GameBoy::GameBoy()
{
	_game.initGame("rom/tetris.gb"); //For testing
	_memory.init(&_game);
	//_video.loadSprite(&_memory);
	//_video.renderSprite();
	_CPU.init(&_memory);
}

GameBoy::~GameBoy()
{

}

void GameBoy::update()
{
	bool run = true;
	while (run)
	{
		int cyclesThisUpdate = 0;
		while (cyclesThisUpdate < MAXCYCLES)
		{
			int cycles = _CPU.fetch();
			if (cycles == -1)
			{
				run = false;
				break;
			}
			cyclesThisUpdate += cycles;
			_CPU.updateTimers(cycles);
			// Graphics
			_CPU.doInterrupts();
		}
		// Render
	}
}
