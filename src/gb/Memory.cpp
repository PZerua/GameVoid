#include "memory.h"

#include "io_registers.h"

void Memory::init(Cartridge *cartridge, Controller *controller)
{
    // Receive Cartridge data
    m_cartridge = cartridge;

    m_controller = controller;

    // Init Memory
    reset();

    m_timerTriger = false;
    m_timerData = 0;
}

BYTE Memory::read(WORD address)
{
    // We are reading data from ROM or internal Cartridge RAM
    if (address < 0x8000 || (address >= 0xA000 && address < 0xC000))
    {
        return m_cartridge->_MBC->read(address);
    }
    // If It's not part of ROM, we read from memory
    else return m_memory[address];
}

void Memory::write(WORD address, BYTE value)
{
    // We are writing data to ROM or internal Cartridge RAM
    if (address < 0x8000 || (address >= 0xA000 && address < 0xC000))
    {
        m_cartridge->_MBC->write(address, value);
        return;
    }
    // In the following two cases we are writing to RAM
    // The internal RAM is echoed from 0xE000 to 0xFE00
    else if (address >= 0xC000 && address < 0xDE00)
    {
        m_memory[address] = value;
        // If we write to internal RAM, value also appears in echoed RAM
        m_memory[address + 0x2000] = value;
    }
    else if (address >= 0xE000 && address < 0xFE00)
    {
        // if we write to echoed RAM, value also appears in internal RAM
        m_memory[address] = value;
        m_memory[address - 0x2000] = value;
    }
    else if (address == 0xFF00)
    {
        m_memory[address] = value;
        m_memory[address] = m_controller->getJoypadState();
    }
    else if (address == DIV)
    {
        m_memory[address] = 0;
        _resetDiv = true;
    }
    // If we write to this registers, they are set to 0
    else if (address == TAC)
    {
        m_timerTriger = true;
        m_timerData = value;
    }
    else if (address == 0xFF44)
        m_memory[0xFF44] = 0;
    // DMA Transfer
    else if (address == 0xFF46)
    {
        DMATransfer(value);
    }
    // Write to memory
    else m_memory[address] = value;
}

void Memory::reset()
{
    // Set all values to 0x00
    memset(m_memory, 0x00, MEM_SIZE);

    // Reset register default values (only those != 0x00)
    m_memory[NR10] = 0x80;
    m_memory[NR11] = 0xBF;
    m_memory[NR12] = 0xF3;
    m_memory[NR14] = 0xBF;
    m_memory[NR21] = 0x3F;
    m_memory[NR24] = 0xBF;
    m_memory[NR30] = 0x7F;
    m_memory[NR31] = 0xFF;
    m_memory[NR32] = 0x9F;
    m_memory[NR33] = 0xBF;
    m_memory[NR41] = 0xFF;
    m_memory[NR44] = 0xBF;
    m_memory[NR50] = 0x77;
    m_memory[NR51] = 0xF3;
    m_memory[NR52] = 0xF1;
    m_memory[LCDC] = 0x91;
    m_memory[BGP]  = 0xFC;
    m_memory[OBP0] = 0xFF;
    m_memory[OBP1] = 0xFF;
    m_memory[TAC]  = 0xF8;
}

void Memory::directModification(WORD address, BYTE value)
{
    m_memory[address] = value;
}

void Memory::DMATransfer(BYTE data)
{
    WORD address = data << 8; // source address is data * 100
    for (int i = 0; i < 0xA0; i++)
    {
        write(0xFE00 + i, read(address + i));
    }
}

bool Memory::timerTriger()
{
    return m_timerTriger;
}

void Memory::resetTimerTriger()
{
    m_timerData = 0;
    m_timerTriger = false;
}

BYTE Memory::getTimerData()
{
    return m_timerData;
}

BYTE *Memory::getMemoryData()
{
    return m_memory;
}