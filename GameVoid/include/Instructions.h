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
	bool hasCarry8(const int &a, const int &b);
	bool hasHalfCarry8(const int &a, const int &b);
	bool hasCarry16(const int &a, const int &b);
	bool hasHalfCarry16(const int &a, const int &b);
	void NOP();
	void LD_n_nn(WORD &n);
	void LD_n_A(const regID &n);
	void INC_nn(WORD &nn);
	void INC_n(const regID &n);
	
private:
	Memory *_memory;
	Registers *_registers;
};