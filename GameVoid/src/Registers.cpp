#include "Registers.h"

Registers::Registers()
{
	reset();
}

Registers::~Registers()
{

}

// Set initial values
void Registers::reset()
{
	AF.nn = 0x01B0;
	BC.nn = 0x0013;
	DE.nn = 0x00D8;
	HL.nn = 0x014D;
	SP = 0xFFFE;
	PC = 0x0100;
}

// Get A from AF (higher 8 bits)
BYTE &Registers::getA()
{
	return AF.n[0];
}

// Get F from AF (lower 8 bits)
BYTE &Registers::getF()
{
	return AF.n[1];
}

// Get B from BC (higher 8 bits)
BYTE &Registers::getB()
{
	return BC.n[0];
}

// Get C from BC (lower 8 bits)
BYTE &Registers::getC()
{
	return BC.n[1];
}

// Get D from DE (higher 8 bits)
BYTE &Registers::getD()
{
	return DE.n[0];
}

// Get E from DE (lower 8 bits)
BYTE &Registers::getE()
{
	return DE.n[1];
}

// Get H from HL (higher 8 bits)
BYTE &Registers::getH()
{
	return HL.n[0];
}

// Get L from HL (lower 8 bits)
BYTE &Registers::getL()
{
	return AF.n[1];
}

WORD &Registers::getAF()
{
	return AF.nn;
}

WORD &Registers::getBC()
{
	return BC.nn;
}

WORD &Registers::getDE()
{
	return DE.nn;
}

WORD &Registers::getHL()
{
	return HL.nn;
}

WORD &Registers::getPC()
{
	return PC;
}

WORD &Registers::getSP()
{
	return SP;
}

// Set higher 8 bits of AF
void Registers::setA(const BYTE &value)
{
	AF.n[0] = value;
}

// Set lower 8 bits of AF
void Registers::setF(const BYTE &value)
{
	AF.n[1] = value;
}

// Zero Flag
void Registers::setF_Z(const bool &value)
{
	AF.n[1] = (AF.n[1] & 0x7F) + (value << 7);
}

// Subtract Flag
void Registers::setF_N(const bool &value)
{
	AF.n[1] = (AF.n[1] & 0xBF) + (value << 6);
}

// Half Carry Flag
void Registers::setF_H(const bool &value)
{
	AF.n[1] = (AF.n[1] & 0xDF) + (value << 5);
}

// Carry Flag
void Registers::setF_C(const bool &value)
{
	AF.n[1] = (AF.n[1] & 0xEF) + (value << 4);
}

// Set higher 8 bits of BC
void Registers::setB(const BYTE &value)
{
	BC.n[0] = value;
}

// Set lower 8 bits of BC
void Registers::setC(const BYTE &value)
{
	BC.n[1] = value;
}

// Set higher 8 bits of DE
void Registers::setD(const BYTE &value)
{
	DE.n[0] = value;
}

// Set lower 8 bits of DE
void Registers::setE(const BYTE &value)
{
	DE.n[1] = value;
}

// Set higher 8 bits of HL
void Registers::setH(const BYTE &value)
{
	HL.n[0] = value;
}

// Set lower 8 bits of HL
void Registers::setL(const BYTE &value)
{
	HL.n[1] = value;
}

void Registers::setAF(const WORD &value)
{
	AF.nn = value;
}

void Registers::setBC(const WORD &value)
{
	BC.nn = value;
}

void Registers::setDE(const WORD &value)
{
	DE.nn = value;
}

void Registers::setHL(const WORD &value)
{
	HL.nn = value;
}

void Registers::setSP(const WORD &value)
{
	SP = value;
}

void Registers::addPC(const int &value)
{
	PC += value;
}

void Registers::setReg(const regID &id, const WORD &value)
{
	switch (id)
	{
	case A:
		setA((BYTE)value);
		break;
	case B:
		setB((BYTE)value);
		break;
	case C:
		setC((BYTE)value);
		break;
	case D:
		setD((BYTE)value);
		break;
	case E:
		setE((BYTE)value);
		break;
	case F:
		setF((BYTE)value);
		break;
	case H:
		setH((BYTE)value);
		break;
	case L:
		setL((BYTE)value);
		break;
	case af:
		setAF(value);
		break;
	case bc:
		setBC(value);
		break;
	case de:
		setDE(value);
		break;
	case hl:
		setHL(value);
		break;
	default:
		throw exception("SetReg error, unimplemented enum");
		break;
	}
}

WORD Registers::getReg(const regID &id)
{
	switch (id)
	{
	case A: return getA();
	case B: return getB();
	case C: return getC();
	case D: return getD();
	case E: return getE();
	case F: return getF();
	case H: return getH();
	case L: return getL();
	case af: return getAF();
	case bc: return getBC();
	case de: return getDE();
	case hl: return getHL();
	default:
		throw exception("GetReg error, unimplemented enum");
		break;
	}
}
