#pragma once
#include "Header.h"
#include "MBC.h"

class None : public MBC
{
public:
	None(BYTE *ROMdata, unsigned RAMsize);
	~None();
	BYTE read(WORD address);
	void write(WORD address, BYTE value);
private:
	BYTE *_ROMdata;
};
