#include "gameboy.h"

#include <sstream>
#include <iomanip>
#include <iostream>

#include "io_registers.h"
#include "interrupts.h"

bool GameBoy::init(const std::string romPath)
{
    if (!cartridge.initGame(romPath)) {
        return false;
    }

    memory.init(&cartridge, &controller);
    video.init(&memory);
    cpu.init(&memory);
    controller.init(memory.getMemoryData());

    return true;
}

void GameBoy::tick(double deltaTime)
{
    unsigned int cyclesExecuted = 0;
    unsigned int cyclesToExecute = static_cast<unsigned int>((deltaTime * 1000000.0) / MICROSECONDS_PER_CYCLE);

    controller.checkControls();

    while (cyclesExecuted < cyclesToExecute)
    {
        if (controller.interruptRequested())
        {
            cpu.requestInterrupt(Interrupt::JoyPad);
            controller.setInterruptRequested(false);
        }

        int cycles = cpu.fetch();
        cpu.updateTimers(cycles);
        video.updateGraphics(cycles, cpu);
        cpu.doInterrupts();
        cyclesExecuted += cycles;
    }

    video.render();
}
