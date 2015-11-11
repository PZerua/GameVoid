#pragma once

#include "Header.h"

enum regID
{
	A, B, C, D, E, F, H, L,
	af, bc, de, hl, sp, pc,
	n8, n16, mBC, mDE, mHL,
	nZ, Z, nC, sC
};

class Registers
{
public:
	Registers();
	~Registers();

	void reset();

	const BYTE getA();
	const BYTE getF();
	const BYTE getF_Z();
	const BYTE getF_N();
	const BYTE getF_H();
	const BYTE getF_C();
	const BYTE getB();
	const BYTE getC();
	const BYTE getD();
	const BYTE getE();
	const BYTE getH();
	const BYTE getL();
	const WORD getAF();
	const WORD getBC();
	const WORD getDE();
	const WORD getHL();
	const WORD getPC();
	const WORD getSP();

	void setA(const BYTE &value);
	void setF(const BYTE &value);
	void setF_Z(const BYTE &value);
	void setF_N(const BYTE &value);
	void setF_H(const BYTE &value);
	void setF_C(const BYTE &value);
	void setB(const BYTE &value);
	void setC(const BYTE &value);
	void setD(const BYTE &value);
	void setE(const BYTE &value);
	void setH(const BYTE &value);
	void setL(const BYTE &value);
	void setAF(const WORD &value);
	void setBC(const WORD &value);
	void setDE(const WORD &value);
	void setHL(const WORD &value);
	void addPC(const BYTE &value);
	void addSP(const BYTE &value);
	void setSP(const WORD &value);
	void setPC(const WORD &value);

	void setReg(const regID &id, const WORD &value);
	const WORD getReg(const regID &id);

private:

	union reg
	{
		WORD nn;
		BYTE n[2];
	};

	reg AF;
	reg BC;
	reg DE;
	reg HL;
	// Stack Pointer
	WORD SP;
	// Program Counter
	WORD PC;
};
