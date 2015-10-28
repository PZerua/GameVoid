#include "None.h"

None::None(BYTE *ROMdata)
{
	_ROMdata = ROMdata;
}

None::~None()
{

}

BYTE None::read(const WORD &address)
{
	// Read ROM data
	if (address >= 0x0000 && address < 0x8000)
	{
		return _ROMdata[address];
	}
	// Read RAM data 
	else if (address >= 0xA000 && address < 0xC000)
	{

	}
}

void None::write(const WORD &address, const BYTE &value)
{
	// Enable or disable RAM
	if (address >= 0x0000 && address < 0x2000)
	{

	}
	// Write to RAM 
	else if (address >= 0xA000 && address < 0xC000)
	{

	}
	else cout << "Error: writing to a ROM without MBC" << endl;
}