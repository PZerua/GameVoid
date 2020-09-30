#include "GameBoy.h"

#include <chrono>
#include <sstream>
#include <iomanip>

GameBoy::GameBoy()
{
}

GameBoy::~GameBoy()
{

}

bool GameBoy::init(const std::string romPath)
{
    if (!_window.init("GameVoid", 640, 576)) {
        return false;
    }

    if (!_game.initGame(romPath)) {
        return false;
    }

    _memory.init(&_game, &_controller);
    _video.init(&_memory);
    _CPU.init(&_memory);
    _controller.init(_memory.getMemoryData());

    return true;
}

void GameBoy::start()
{
    input::Keyboard &input = input::Keyboard::getInstance();

    bool run = true;

    while (run && !input.isPressed(GLFW_KEY_ESCAPE))
    {
        int cyclesThisUpdate = 0;

        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        _window.pollEvents();
        _controller.checkControls();

        while (cyclesThisUpdate < MAXCYCLES)
        {
            if (_controller.interruptRequested())
            {
                _CPU.requestInterrupt(Interrupt::JoyPad);
                _controller.setInterruptRequested(false);
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
                    //_CPU.setClockFreq();
                }
                _memory.resetTimerTriger();
            }
            if (_memory._resetDiv)
            {
                _CPU.m_divideCounter = 0;
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

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << 1000.0 / std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count();
        _window.setWindowTitle("GameVoid " + stream.str());
        //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[ms]" << std::endl;
    }
}
