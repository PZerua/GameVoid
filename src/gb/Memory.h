#pragma once

#include "data_types.h"
#include "cartridge.h"
#include "controller.h"

#define MEM_SIZE 65536

class Memory
{
public:
    Memory() {}
    ~Memory() {}
    void init(Cartridge *cartridge, Controller *controller);
    void reset();
    BYTE read(WORD address);
    void write(WORD address, BYTE value);
    void directModification(WORD address, BYTE value);
    void DMATransfer(BYTE data);
    bool timerTriger();
    void resetTimerTriger();
    BYTE getTimerData();
    bool _resetDiv;
    BYTE *getMemoryData();

private:
    BYTE        m_memory[MEM_SIZE];
    Cartridge*  m_cartridge;
    bool        m_timerTriger;
    BYTE        m_timerData;
    Controller* m_controller;
};
