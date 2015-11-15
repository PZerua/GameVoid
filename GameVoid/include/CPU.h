#pragma once

#include "Header.h"
#include "Memory.h"
#include "Registers.h"
#include "Instructions.h"

class CPU
{
public:
	CPU() {}
	~CPU() {}
	void init(Memory *memory);
	void run();
	void CB_prefix(Instructions *inst);
	void initCyclesArrays();
	void condCycles();
	void uncondCycles();

private:
	Registers _registers;
	Memory *_memory;
	Instructions *_inst;

	short _instCycles[0x100];
	short _CBinstCycles[0x100];
	BYTE IME;
	bool _condTaken;
};
