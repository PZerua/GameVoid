#include "Memory.h"

void Memory::init(Cartridge *cartridge)
{
	// Receibe Cartridge data
	_cartridge = cartridge;

	// Init Memory
	reset();
}

BYTE Memory::read(const WORD &address)
{
	// We are reading data from ROM or internal Cartridge RAM
	if (address < 0x8000 || (address >= 0xA000 && address < 0xC000))
	{
		return _cartridge->_MBC->read(address);
	}
	// If It's not part of ROM, we read from memory
	else return _MEMORY[address];
}

void Memory::write(const WORD &address, const BYTE &value)
{
	// We are writing data to ROM or internal Cartridge RAM
	if (address < 0x8000 || (address >= 0xA000 && address < 0xC000))
	{
		_cartridge->_MBC->write(address, value);
		return;
	}
	// In the following two cases we are writing to RAM
	// The internal RAM is echoed from 0xE000 to 0xFE00
	else if (address >= 0xC000 && address < 0xE000)
	{
		// If we write to internal RAM, value also appears in echoed RAM
		_MEMORY[address + 0x2000] = value;
	}
	else if (address >= 0xE000 && address < 0xFE00)
	{
		// if we write to echoed RAM, value also appears in internal RAM
		_MEMORY[address - 0x2000] = value;
	}
	else if (address == 0xFF04)
		_MEMORY[0xFF04] = 0;

	// Write to memory
	_MEMORY[address] = value;
}

void Memory::reset()
{
	// Set all values to 0x00
	memset(_MEMORY, 0x00, MEM_SIZE);

	// Reset register default values (only those != 0x00)
	_MEMORY[NR10] = 0x80;
	_MEMORY[NR11] = 0xBF;
	_MEMORY[NR12] = 0xF3;
	_MEMORY[NR14] = 0xBF;
	_MEMORY[NR21] = 0x3F;
	_MEMORY[NR24] = 0xBF;
	_MEMORY[NR30] = 0x7F;
	_MEMORY[NR31] = 0xFF;
	_MEMORY[NR32] = 0x9F;
	_MEMORY[NR33] = 0xBF;
	_MEMORY[NR41] = 0xFF;
	_MEMORY[NR44] = 0xBF;
	_MEMORY[NR50] = 0x77;
	_MEMORY[NR51] = 0xF3;
	_MEMORY[NR52] = 0xF1;
	_MEMORY[LCDC] = 0x91;
	_MEMORY[LY]   = 0x90; // TODO: Find info
	_MEMORY[BGP]  = 0xFC;
	_MEMORY[OBP0] = 0xFF;
	_MEMORY[OBP1] = 0xFF;
}

void Memory::directModification(const WORD &address, const BYTE &value)
{
	_MEMORY[address] = value;
}