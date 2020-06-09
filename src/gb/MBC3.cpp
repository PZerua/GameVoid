#include "MBC3.h"

MBC3::MBC3(BYTE *ROMdata, unsigned RAMsize)
{
	_ROMdata = ROMdata;
	memset(_RAMdata, 0, RAMsize);
	_RAMsize = RAMsize;
	_RAMenabled = false;
	_mode = 0;
	_ROMbank = 1;
	_RAMbank = 0;
}

MBC3::~MBC3()
{

}

BYTE MBC3::read(WORD address)
{
	// Read ROM Bank 0
	if (address >= 0x0000 && address < 0x4000)
	{
		return _ROMdata[address];
	}
	// Read ROM Bank 0x01 to 0x7f (up to 255)
	else if (address >= 0x4000 && address < 0x8000)
	{
		return _ROMdata[(0x4000 * _ROMbank) + (address - 0x4000)];
	}
	// Read RAM Bank 0x00 to 0x03 
	else if (address >= 0xA000 && address < 0xC000)
	{
		if (_RAMsize == 0 || !_RAMenabled)
		{
			cout << hex << "PC: " << (int)lastPC << " | OPCODE: " << (int)lastOPCODE << endl;
			throw exception("Trying to read uninitialized or null RAM");
		}
		return _RAMdata[(0x2000 * _RAMbank) + (address - 0xA000)];
	}
	else
	{
		cout << hex << "PC: " << (int)lastPC << " | OPCODE: " << (int)lastOPCODE << endl;
		throw exception("Wrong address");
	}
}

void MBC3::write(WORD address, BYTE value)
{
	// Enable or disable RAM
	if (address >= 0x0000 && address < 0x2000)
	{
		// Any value with 0x0A in lower 4 bits enables RAM
		if ((value & 0x0F) == 0x0A)
		{
			_RAMenabled = true;
		}
		// Other value disables it
		else
		{
			_RAMenabled = false;
		}
	}
	// Select ROM Bank Number (only lower 5 bits)
	else if (address >= 0x2000 && address < 0x4000)
	{
		BYTE tempValue = value;
		if ((value & 0x1F) == 0)
			tempValue++;
		_ROMbank = (tempValue & 0x7F) | (_ROMbank & 0x80);
	}
	// Select RAM Bank Number or upper bits of ROM Bank Number
	else if (address >= 0x4000 && address < 0x6000)
	{
		if (_mode == ROMmode)
			_ROMbank = (_ROMbank & 0x1F) | ((value & 0x03) << 5);
		else if (_mode == RAMmode)
			_RAMbank = value & 0x03;
	}
	// Select ROM or RAM mode
	else if (address >= 0x6000 && address < 0x8000)
	{
		if (value == 0x00)
			_mode = ROMmode;
		else if (value == 0x01)
			_mode = RAMmode;
	}
	// Write RAM Bank 0x00 to 0x03 
	else if (address >= 0xA000 && address < 0xC000)
	{
		if (_RAMsize != 0 && _RAMenabled)
		{
			_RAMdata[(0x2000 * _RAMbank) + (address - 0xA000)] = value;
		}
	}
}
