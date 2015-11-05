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
	bool hasBorrow8(const int &a, const int &b);
	bool hasHalfBorrow8(const int &a, const int &b);
	bool hasBorrow16(const int &a, const int &b);
	bool hasHalfBorrow16(const int &a, const int &b);
	void NOP();
	void LD_nn_n(const regID &nn);
	void LD_n_nn(const regID &n);
	void LD_n_A(const regID &n);
	void LD_A_n(const regID &n);
	void LD_nn_SP();
	void ADD_HL_n(const regID &n);
	void INC_nn(const regID &n);
	void INC_n(const regID &n);
	void DEC_n(const regID &n);
	void RLCA();
	void RLC();
	
private:
	Memory *_memory;
	Registers *_registers;
};