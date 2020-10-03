#pragma once

#include "interrupts.h"
#include "cpu_registers.h"

class Memory;
class Instructions;

class CPU
{

public:

    CPU() = default;
    void init(Memory *memory);
    int fetch();
    int CB_prefix(Instructions *inst);
    void initCyclesArrays();
    void condCycles();
    void uncondCycles();
    void requestInterrupt(Interrupt id);
    void doInterrupts();
    void serviceInterrupt(Interrupt id);
    int getClockFreq() const;
    void updateTimers(int cycles);
    bool isClockEnabled();
    int getDivideCounter() { return m_divideCounter; }
    void resetTimeCounter() { m_timeCounter = 0; }
    void resetDivideCounter() { m_divideCounter = 0; }

private:

    Registers       m_registers;
    Memory*         m_memory = nullptr;
    Instructions*   m_inst = nullptr;

    short   m_instCycles[0x100];
    short   m_CBinstCycles[0x100];
    bool    m_IME = true;  // Interrupt master enable
    int     m_timeCounter = 0;
    int     m_divideCounter = 0;
    bool    m_haltEnabled = false;
};
