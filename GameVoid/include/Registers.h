#pragma once

#include "Header.h"

enum regID
{
	A, B, C, D, E, F, H, L,
	af, bc, de, hl, sp, pc,
	nn, mBC, mDE, mHL
};

class Registers
{
public:
	Registers();
	~Registers();

	void reset();

	BYTE &getA();
	BYTE &getF();
	BYTE &getB();
	BYTE &getC();
	BYTE &getD();
	BYTE &getE();
	BYTE &getH();
	BYTE &getL();
	WORD &getAF();
	WORD &getBC();
	WORD &getDE();
	WORD &getHL();
	WORD &getPC();
	WORD &getSP();

	void setA(const BYTE &value);
	void setF(const BYTE &value);
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
	void addPC(const int &value);
	void setSP(const WORD &value);

	void setReg(const regID &id, const WORD &value);

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
