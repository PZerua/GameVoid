#include "GameBoy.h"

GameBoy::GameBoy()
{
	_game.initGame("rom/kwirk.gb"); //For testing
	_memory.init(&_game, &_controller);
	_video.init(&_memory);
	_CPU.init(&_memory);
	_controller.init(_memory.getMemoryData());
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
			SDL_PollEvent(&event);
			_controller.checkControls(&event);
			if (_controller.requestInterrupt())
			{
				_CPU.requestInterrupt(4);
				_controller.setRequestInterrupt(false);
			}
			int cycles = _CPU.fetch();
			if (_memory.timerTriger())
			{
				BYTE currentfreq = _CPU.getClockFreq();
				BYTE data = _memory.getTimerData();
				_memory.directModification(TAC, data);
				BYTE newfreq = _CPU.getClockFreq();

				if (currentfreq != newfreq)
				{
					_CPU.setClockFreq();
				}
				_memory.resetTimerTriger();
			}
			if (_memory._resetDiv)
			{
				_CPU._divideCounter = 0;
				_memory._resetDiv = false;
			}
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
		_video.render();
	}
}
