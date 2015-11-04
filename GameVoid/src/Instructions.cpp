#include "Instructions.h"

Instructions::Instructions(Memory *memory, Registers *registers)
{
	_memory = memory;
	_registers = registers;
}

Instructions::~Instructions()
{

}

// Read immediate 16 bits value
WORD Instructions::read16()
{
	BYTE low = _memory->read(_registers->getPC() + 1);
	BYTE high = _memory->read(_registers->getPC() + 2);
	WORD address = low + (high << 8);

	return address;
}

// Read immediate 8 bits value
BYTE Instructions::read8()
{
	return _memory->read(_registers->getPC() + 1);
}

bool Instructions::hasHalfCarry8(const int &a, const int &b)
{
	if ((((a & 0x000F) + (b & 0x000F)) & 0x10) == 0x10)
		return 1;
	else return 0;
}

bool Instructions::hasCarry8(const int &a, const int &b)
{
	if ((((a & 0x00FF) + (b & 0x00FF)) & 0x100) == 0x100)
		return 1;
	else return 0;
}

bool Instructions::hasHalfCarry16(const int &a, const int &b)
{
	if ((((a & 0x0FFF) + (b & 0x0FFF)) & 0x1000) == 0x1000)
		return 1;
	else return 0;
}

bool Instructions::hasCarry16(const int &a, const int &b)
{
	if ((((a & 0xFFFF) + (b & 0xFFFF)) & 0x10000) == 0x10000)
		return 1;
	else return 0;
}

// 0x00
void Instructions::NOP()
{
	_registers->addPC(1);
}

// 0x01, 0x11, 0x21, 0x31
// n = BC, DE, HL, SP
// nn = 16 bit value
void Instructions::LD_n_nn(WORD &n)
{
	// nn = read16()
	n = read16();
	_registers->addPC(3);
}

// 0x02, 0x12, 0x47, 0x4F, 0x57, 0x5F, 0x67, 0x6F, 0x77. 0x7F, 0xEA
// n = A, B, C, D, E, H, L, (BC), (DE), (HL), (nn, inm 16)
void Instructions::LD_n_A(const regID &n)
{
	switch (n)
	{
	case mBC:
		_memory->write(_registers->getBC(), _registers->getA());
		break;
	case mDE:
		_memory->write(_registers->getDE(), _registers->getA());
		break;
	case mHL:
		_memory->write(_registers->getHL(), _registers->getA());
		break;
	case nn:
		_memory->write(read16(), _registers->getA());
		break;
	default:
		// For n = A, B, C, D, E, H, L 
		_registers->setReg(n, _registers->getA());
		break;
	}

	_registers->addPC(1);
}

// 0x03, 0x13, 0x23, 0x33
// nn = BC, DE, HL, SP
void Instructions::INC_nn(WORD &nn)
{
	nn++;
	_registers->addPC(1);
}

// 0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C, 0x34, 0x3C
// n = A, B, C, D, E, H, L, (HL)
void Instructions::INC_n(const regID &n)
{

	if (hasHalfCarry8(n, 1))
		_registers->setF_H(1);
	else _registers->setF_H(0);

	switch (n)
	{
	case mHL:
		_memory->write(_registers->getHL(), _memory->read(_registers->getHL()) + 1);
		break;
	default:
		_registers->setReg(n, _registers->getReg(n) + 1);
		break;
	}

	if (n == 0)
		_registers->setF_Z(1);
	else _registers->setF_Z(0);

	_registers->setF_N(0);

	_registers->addPC(1);
}