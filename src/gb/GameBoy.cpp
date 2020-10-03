#include "gameboy.h"

#include <chrono>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "io_registers.h"
#include "interrupts.h"

bool GameBoy::init(const std::string romPath)
{
    if (!m_window.init("GameVoid", 640, 576)) {
        return false;
    }

    if (!m_game.initGame(romPath)) {
        return false;
    }

    m_memory.init(&m_game, &m_controller);
    m_video.init(&m_memory);
    m_cpu.init(&m_memory);
    m_controller.init(m_memory.getMemoryData());

    return true;
}

void GameBoy::start()
{
    input::Keyboard &input = input::Keyboard::getInstance();

    while (!input.isPressed(GLFW_KEY_ESCAPE) && !m_window.isClosed())
    {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        unsigned int cyclesExecuted = 0;
        unsigned int cyclesToExecute = static_cast<unsigned int>((m_deltaTime * 1000000.0) / MICROSECONDS_PER_CYCLE);

        m_window.pollEvents();
        m_controller.checkControls();

        while (cyclesExecuted < cyclesToExecute)
        {
            if (m_controller.interruptRequested())
            {
                m_cpu.requestInterrupt(Interrupt::JoyPad);
                m_controller.setInterruptRequested(false);
            }
            int cycles = m_cpu.fetch();
            m_cpu.updateTimers(cycles);
            m_video.updateGraphics(cycles, m_cpu);
            m_cpu.doInterrupts();
            cyclesExecuted += cycles;
        }

        m_window.clear();
        m_video.render();
        m_window.swap();

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        std::stringstream stream;
        m_deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() * 0.001;
        stream << std::fixed << std::setprecision(2) << 1.0 / m_deltaTime;
        m_window.setWindowTitle("GameVoid " + stream.str());
        //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[ms]" << std::endl;
    }
}
