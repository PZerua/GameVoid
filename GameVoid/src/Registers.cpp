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
	AF = 0x01B0;
	BC = 0x0013;
	DE = 0x00D8;
	HL = 0x014D;
	SP = 0xFFFE;
	PC = 0x0100;
}

// Get A from AF (higher 8 bits)
BYTE Registers::getA()
{
	return (AF & 0xFF00);
}

// Get F from AF (lower 8 bits)
BYTE Registers::getF()
{
	return (AF & 0x00FF);
}

// Get B from BC (higher 8 bits)
BYTE Registers::getB()
{
	return (AF & 0xFF00);
}

// Get C from BC (lower 8 bits)
BYTE Registers::getC()
{
	return (AF & 0x00FF);
}

// Get D from DE (higher 8 bits)
BYTE Registers::getD()
{
	return (AF & 0xFF00);
}

// Get E from DE (lower 8 bits)
BYTE Registers::getE()
{
	return (AF & 0x00FF);
}

// Get H from HL (higher 8 bits)
BYTE Registers::getH()
{
	return (AF & 0xFF00);
}

// Get L from HL (lower 8 bits)
BYTE Registers::getL()
{
	return (AF & 0x00FF);
}

WORD Registers::getAF()
{
	return AF;
}

WORD Registers::getBC()
{
	return BC;
}

WORD Registers::getDE()
{
	return DE;
}

WORD Registers::getHL()
{
	return HL;
}

WORD Registers::getPC()
{
	return PC;
}

// Set higher 8 bits of AF
void Registers::setA(const BYTE &value)
{
	AF = (AF & 0x00FF) + (value << 8);
}

// Set lower 8 bits of AF
void Registers::setF(const BYTE &value)
{
	AF = (AF & 0xFF00) + value;
}

// Set higher 8 bits of BC
void Registers::setB(const BYTE &value)
{
	BC = (BC & 0x00FF) + (value << 8);
}

// Set lower 8 bits of BC
void Registers::setC(const BYTE &value)
{
	BC = (BC & 0xFF00) + value;
}

// Set higher 8 bits of DE
void Registers::setD(const BYTE &value)
{
	DE = (DE & 0x00FF) + (value << 8);
}

// Set lower 8 bits of DE
void Registers::setE(const BYTE &value)
{
	DE = (DE & 0xFF00) + value;
}

// Set higher 8 bits of HL
void Registers::setH(const BYTE &value)
{
	HL = (HL & 0x00FF) + (value << 8);
}

// Set lower 8 bits of HL
void Registers::setL(const BYTE &value)
{
	HL = (HL & 0xFF00) + value;
}

void Registers::setAF(const WORD &value)
{
	AF = value;
}

void Registers::setBC(const WORD &value)
{
	BC = value;
}

void Registers::setDE(const WORD &value)
{
	DE = value;
}

void Registers::setHL(const WORD &value)
{
	HL = value;
}

void Registers::addPC(const int &value)
{
	PC += value;
}