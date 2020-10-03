#pragma once

#include "data_types.h"

#define MEM_SIZE 65536

class Cartridge;
class Controller;

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
    BYTE *getMemoryData();

private:
    BYTE        m_memory[MEM_SIZE];
    Cartridge*  m_cartridge = nullptr;
    Controller* m_controller = nullptr;
};
