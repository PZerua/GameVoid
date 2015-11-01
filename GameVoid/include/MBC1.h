#pragma once
#include "Header.h"
#include "MBC.h"

class MBC1 : public MBC
{
public:
	MBC1(BYTE *ROMdata, const unsigned &RAMsize);
	~MBC1();
	BYTE read(const WORD &address);
	void write(const WORD &address, const BYTE &value);
private:
	BYTE *_ROMdata;
};
