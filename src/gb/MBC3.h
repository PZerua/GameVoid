#pragma once

#pragma once
#include "Header.h"
#include "MBC.h"

class MBC3 : public MBC
{
public:
	MBC3(BYTE *ROMdata, unsigned RAMsize);
	~MBC3();
	BYTE read(WORD address);
	void write(WORD address, BYTE value);
private:
	BYTE *_ROMdata;
	BYTE _RAMdata[0x8000];
};
