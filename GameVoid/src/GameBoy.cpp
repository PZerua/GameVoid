#include "GameBoy.h"

GameBoy::GameBoy()
{
	_game.initGame("rom/bubble.gb"); //For testing
	_memory.init(&_game);
	_video.init(&_memory);
	_CPU.init(&_memory);
}

GameBoy::~GameBoy()
{

}

void GameBoy::update()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	bool run = true;
	while (run && event.type != SDL_QUIT)
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
			_video.updateGraphics(cycles, _CPU);
			_CPU.doInterrupts();
		}
		SDL_PollEvent(&event);
		_video.render();
	}
}
