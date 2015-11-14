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
	void LD_HLI_A();
	void LD_HLD_A();
	void LD_A_HLI();
	void LD_r1_r2(const regID &r1, const regID &r2);
	void LDH_n_A();
	void LDH_A_n();
	void ADD_HL_n(const regID &n);
	void ADD_A_n(const regID &n);
	void ADC_A_n(const regID &n);
	void INC_nn(const regID &n);
	void INC_n(const regID &n);
	void SUB_A_n(const regID &n);
	void SBC_A_n(const regID &n);
	void DEC_nn(const regID &nn);
	void DEC_n(const regID &n);
	void RLCA();
	void RLA();
	void RRCA();
	void RRA();
	void STOP();
	void JR_n();
	void JR_cc_n(const regID &n);
	void JP_cc_nn(const regID &cc);
	void DAA();
	void CPL();
	void HALT();
	void JP_nn();
	void AND_n(const regID &n);
	void XOR_n(const regID &n);
	void OR_n(const regID &n);
	void CP_n(const regID &n);
	void RET_cc(const regID &cc);
	void RET();
	void POP_nn(const regID &nn);
	void CALL_nn();
	void CALL_cc_nn(const regID &cc);
	void DI();
	void EI();
	void SCF();
	void CCF();
	void PUSH_nn(const regID &nn);
	void RST_n(const BYTE &n);
	
private:
	Memory *_memory;
	Registers *_registers;
};
