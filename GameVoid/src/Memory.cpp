#include "Memory.h"

Memory::Memory(Cartridge *cartridge)
{
	_cartridge = cartridge;
}

Memory::~Memory()
{

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
		_MEMORY[address + 0x4000] = value;
	}
	else if (address >= 0xE000 && address < 0xFE00)
	{
		// if we write to echoed RAM, value also appears in internal RAM
		_MEMORY[address - 0x4000] = value;
	}

	// Write to memory
	_MEMORY[address] = value;
}