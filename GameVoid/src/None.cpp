#include "None.h"

None::None(BYTE *ROMdata, const unsigned &RAMsize)
{
	_ROMdata = ROMdata;
	_RAMenabled = false;
	_RAMsize = RAMsize;
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
		if (_RAMsize == 0 || !_RAMenabled)
			throw exception("Trying to read uninitialized or null RAM");
		return _ROMdata[address];
	}
	else throw exception("Wrong address");
}

void None::write(const WORD &address, const BYTE &value)
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
	// Write to RAM 
	else if (address >= 0xA000 && address < 0xC000)
	{
		if (_RAMsize != 0 && _RAMenabled)
			_ROMdata[address] = value;
	}
	//else cout << "Error: writing to a ROM without MBC" << endl;
}
