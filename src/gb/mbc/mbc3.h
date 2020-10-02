#pragma once

#include "mbc.h"

class MBC3 : public MBC
{
public:
	MBC3(BYTE *ROMdata, unsigned RAMsize);
	~MBC3();
	BYTE read(WORD address);
	void write(WORD address, BYTE value);
private:
	BYTE* m_ROMdata = nullptr;
	BYTE  m_RAMdata[0x8000] = {};
};
