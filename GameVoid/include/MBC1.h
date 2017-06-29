#pragma once
#include "Header.h"
#include "MBC.h"

class MBC1 : public MBC
{
public:
	MBC1(BYTE *ROMdata, unsigned RAMsize);
	~MBC1();
	BYTE read(WORD address);
	void write(WORD address, BYTE value);
private:
	BYTE *_ROMdata;
	BYTE _RAMdata[0x8000];
};
