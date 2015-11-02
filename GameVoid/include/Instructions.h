#pragma once

#include "Header.h"
#include "Registers.h"
#include "Memory.h"

class Instructions
{
public:
	Instructions(Memory *memory, Registers *registers);
	~Instructions();

	WORD read16();
	BYTE read8();
	void NOP();
	void LD_n_nn(WORD &n);
	void LD_n_A(const regID &id);
	
private:
	Memory *_memory;
	Registers *_registers;
};