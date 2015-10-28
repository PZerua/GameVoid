#include "MBC1.h"

MBC1::MBC1(BYTE *ROMdata)
{
	_ROMdata = ROMdata;
}

MBC1::~MBC1()
{

}

BYTE MBC1::read(const WORD &address)
{
	// Read ROM Bank 0
	if (address >= 0x0000 && address < 0x4000)
	{
		return _ROMdata[address];
	}
	// Read ROM Bank 0x01 to 0x7f (up to 255)
	else if (address >= 0x4000 && address < 0x8000)
	{

	}
	// Read RAM Bank 0x00 to 0x03 
	else if (address >= 0xA000 && address < 0xC000)
	{

	}
}

void MBC1::write(const WORD &address, const BYTE &value)
{
	// Enable or disable RAM
	if (address >= 0x0000 && address < 0x2000)
	{

	}
	// Select ROM Bank Number
	else if (address >= 0x2000 && address < 0x4000)
	{

	}
	// Select RAM Bank Number of upper bits of ROM Bank Number
	else if (address >= 0x4000 && address < 0x6000)
	{

	}
	// Select ROM or RAM mode
	else if (address >= 0x6000 && address < 0x8000)
	{

	}
}