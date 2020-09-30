#pragma once

#include "Header.h"
#include "Memory.h"
#include "Registers.h"
#include "Instructions.h"

class CPU
{
public:
    CPU() = default;
    ~CPU() = default;
    void init(Memory *memory);
    int fetch();
    int CB_prefix(Instructions *inst);
    void initCyclesArrays();
    void condCycles();
    void uncondCycles();
    void requestInterrupt(Interrupt id);
    void doInterrupts();
    void serviceInterrupt(Interrupt id);
    BYTE getClockFreq() const;
    void updateTimers(int cycles);
    bool isClockEnabled();
    int m_divideCounter;

private:
    Registers       m_registers;
    Memory*         m_memory;
    Instructions*   m_inst;

    short   m_instCycles[0x100];
    short   m_CBinstCycles[0x100];
    bool    m_IME;  // Interrupt master enable
    int     m_timeCounter;
    
};
