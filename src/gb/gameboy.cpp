#include "gameboy.h"

#include <chrono>
#include <sstream>
#include <iomanip>

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

    bool run = true;

    while (run && !input.isPressed(GLFW_KEY_ESCAPE) && !m_window.isClosed())
    {
        int cyclesThisUpdate = 0;

        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        m_window.pollEvents();
        m_controller.checkControls();

        while (cyclesThisUpdate < MAXCYCLES)
        {
            if (m_controller.interruptRequested())
            {
                m_cpu.requestInterrupt(Interrupt::JoyPad);
                m_controller.setInterruptRequested(false);
            }
            int cycles = m_cpu.fetch();
            if (m_memory.timerTriger())
            {
                BYTE currentfreq = m_cpu.getClockFreq();
                BYTE data = m_memory.getTimerData();
                m_memory.directModification(TAC, data);
                BYTE newfreq = m_cpu.getClockFreq();

                if (currentfreq != newfreq)
                {
                    //_CPU.setClockFreq();
                }
                m_memory.resetTimerTriger();
            }
            if (m_memory._resetDiv)
            {
                m_cpu.setDivideCounter(0);
                m_memory._resetDiv = false;
            }
            if (cycles == -1)
            {
                run = false;
                break;
            }
            cyclesThisUpdate += cycles;
            m_cpu.updateTimers(cycles);
            m_video.updateGraphics(cycles, m_cpu);
            m_cpu.doInterrupts();
        }

        m_window.clear();
        m_video.render();
        m_window.swap();

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << 1000.0 / std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count();
        m_window.setWindowTitle("GameVoid " + stream.str());
        //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[ms]" << std::endl;
    }
}
