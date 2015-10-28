#pragma once
#include "Header.h"
#include "MBC.h"

class None : public MBC
{
public:
	None(BYTE *ROMdata, const unsigned &RAMsize);
	~None();
	BYTE read(const WORD &address);
	void write(const WORD &address, const BYTE &value);
private:
	BYTE *_ROMdata;
};