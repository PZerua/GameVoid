#include "GameBoy.h"

GameBoy::GameBoy()
{
}

GameBoy::~GameBoy()
{

}

bool GameBoy::init()
{
    if (!_window.init("GameVoid", 640, 576)) {
        return false;
    }

    if (!_game.initGame("rom/zelda.gb")) {
        return false;
    }

    _memory.init(&_game, &_controller);
    _video.init(&_memory);
    _CPU.init(&_memory);
    _controller.init(_memory.getMemoryData());

    return true;
}

void GameBoy::update()
{
    input::Keyboard &input = input::Keyboard::getInstance();

    bool run = true;

    while (run && !input.isPressed(GLFW_KEY_ESCAPE))
    {
        int cyclesThisUpdate = 0;

        while (cyclesThisUpdate < MAXCYCLES)
        {
            _window.pollEvents();
            _controller.checkControls();
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

        _window.clear();
        _video.render();
        _window.swap();
    }
}
