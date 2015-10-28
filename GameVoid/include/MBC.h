#pragma once
#include "Header.h"

class MBC
{
public:
	virtual BYTE read(const WORD &address) = 0;
	virtual void write(const WORD &adress, const BYTE &value) = 0;

protected:
	unsigned _RAMsize;
	bool _RAMenabled;
	bool _mode;
	unsigned _ROMbank;
	unsigned _RAMbank;

	enum {
		ROMmode,
		RAMmode
	};
};