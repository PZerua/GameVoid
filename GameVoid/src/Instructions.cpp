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

bool Instructions::hasHalfBorrow8(const int &a, const int &b)
{
	if ((((a & 0x000F) - (b & 0x000F)) & 0x10) == 0x10)
		return 1;
	else return 0;
}

bool Instructions::hasBorrow8(const int &a, const int &b)
{
	if ((((a & 0x00FF) - (b & 0x00FF)) & 0x100) == 0x100)
		return 1;
	else return 0;
}

bool Instructions::hasHalfBorrow16(const int &a, const int &b)
{
	if ((((a & 0x0FFF) - (b & 0x0FFF)) & 0x1000) == 0x1000)
		return 1;
	else return 0;
}

bool Instructions::hasBorrow16(const int &a, const int &b)
{
	if ((((a & 0xFFFF) - (b & 0xFFFF)) & 0x10000) == 0x10000)
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
void Instructions::LD_n_nn(const regID &n)
{
	// nn = read16()
	_registers->setReg(n, read16());
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
	case n16:
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
void Instructions::INC_nn(const regID &nn)
{
	_registers->setReg(nn, _registers->getReg(nn) + 1);
	_registers->addPC(1);
}

// 0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C, 0x34, 0x3C
// n = A, B, C, D, E, H, L, (HL)
void Instructions::INC_n(const regID &n)
{
	// Set carry in flag
	_registers->setF_H(hasHalfCarry8(_registers->getReg(n), 1));

	switch (n)
	{
	case mHL:
		_memory->write(_registers->getHL(), _memory->read(_registers->getHL()) + 1);
		break;
	default:
		_registers->setReg(n, _registers->getReg(n) + 1);
		break;
	}

	// Set Z and N in flag
	_registers->setF_Z(n == 0);
	_registers->setF_N(0);

	_registers->addPC(1);
}

// 0x05, 0x0D, 0x15, 0x1D, 0x25, 0x2D, 0x35, 0x3D
void Instructions::DEC_n(const regID &n)
{
	// Set carry in flag
	_registers->setF_H(hasHalfBorrow8(_registers->getReg(n), 1));

	switch (n)
	{
	case mHL:
		_memory->write(_registers->getHL(), _memory->read(_registers->getHL()) - 1);
		break;
	default:
		_registers->setReg(n, _registers->getReg(n) + 1);
		break;
	}

	// Set Z and N in flag
	_registers->setF_Z(n == 0);
	_registers->setF_N(1);

	_registers->addPC(1);
}

// 0x06, 0x0E, 0x16, 0x1E, 0x26, 0x2E
// nn = B, C, D, E, H, L
// n = immediate 8 bits
void Instructions::LD_nn_n(const regID &nn)
{
	_registers->setReg(nn, read8());
	_registers->addPC(2);
}

// 0x07
void Instructions::RLCA()
{
	// Set bit 7 as carry
	_registers->setF_C((_registers->getA() & 0x80) >> 7);
	// Rotate
	_registers->setA(((_registers->getA() << 1) & 0xFE) + _registers->getF_C());
	// Set flag values
	_registers->setF_N(0);
	_registers->setF_H(0);

	_registers->addPC(1);
}

//0x08
// nn = 16 bits address
void Instructions::LD_nn_SP()
{
	_memory->write(read16(), _registers->getSP() & 0x00FF);
	_memory->write(read16() + 1, _registers->getSP() & 0xFF00 >> 8);
	_registers->addPC(3);
}

// 0x09, 0x19, 0x29, 0x39
void Instructions::ADD_HL_n(const regID &n)
{

	// Set carries in flag
	_registers->setF_C(hasCarry16(_registers->getHL(), _registers->getReg(n)));
	_registers->setF_H(hasHalfCarry16(_registers->getHL(), _registers->getReg(n)));

	// Add values
	_registers->setHL(_registers->getHL() + _registers->getReg(n));

	// Reset flag value N
	_registers->setF_N(0);

	_registers->addPC(1);
}

// 0x0A, 0x1A, 0x3E, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, 0xFA
void Instructions::LD_A_n(const regID &n)
{
	switch (n)
	{
	case mBC:
		_registers->setA(_memory->read(_registers->getBC()));
		break;
	case mDE:
		_registers->setA(_memory->read(_registers->getDE()));
		break;
	case mHL:
		_registers->setA(_memory->read(_registers->getHL()));
		break;
	case n8:
		_registers->setA(read8());
		break;
	case n16:
		_registers->setA(_memory->read(read16()));
		break;
	default:
		_registers->setA((BYTE)_registers->getReg(n));
		break;
	}

	_registers->addPC(1);
}

// 0x0B
void Instructions::DEC_nn(const regID &nn)
{
	_registers->setReg(nn, _registers->getReg(nn) - 1);
	_registers->addPC(1);
}

// 0x0F
void Instructions::RRCA()
{
	// Set bit 0 as carry
	_registers->setF_C(_registers->getA() & 0x01);
	// Rotate
	_registers->setA(((_registers->getA() >> 1) & 0x7F) + (_registers->getF_C() << 7));
	// Set flag values
	_registers->setF_N(0);
	_registers->setF_H(0);

	_registers->addPC(1);
}

// 0x10
void Instructions::STOP()
{
	// TODO
	_registers->addPC(2);
}

// 0x17
void Instructions::RLA()
{
	// Save previous carry
	BYTE prevC = (_registers->getF() & 0x10) >> 4;
	// Set bit 7 as carry
	_registers->setF_C((_registers->getA() & 0x80) >> 7);
	// Rotate
	_registers->setA(((_registers->getA() << 1) & 0xFE) + prevC);
	// Set flag values
	_registers->setF_N(0);
	_registers->setF_H(0);

	_registers->addPC(1);
}

// 0x18
void Instructions::JR_n()
{
	_registers->addPC(read8());
}

// 0x1F
void Instructions::RRA()
{
	// Save previous carry
	BYTE prevC = (_registers->getF() & 0x10) >> 4;
	// Set bit 0 as carry
	_registers->setF_C(_registers->getA() & 0x01);
	// Rotate
	_registers->setA(((_registers->getA() >> 1) & 0xFE) + (prevC << 7));
	// Set flag values
	_registers->setF_N(0);
	_registers->setF_H(0);

	_registers->addPC(1);
}

// 0x20, 0x28, 0x30, 0x38
void Instructions::JR_cc_n(const regID &id)
{
	switch (id)
	{
	case nZ:
		if (_registers->getF_Z() == 0)
			_registers->addPC(read8());
		else _registers->addPC(2);
		break;
	case Z:
		if (_registers->getF_Z() == 1)
			_registers->addPC(read8());
		else _registers->addPC(2);
		break;
	case nC:
		if (_registers->getF_C() == 0)
			_registers->addPC(read8());
		else _registers->addPC(2);
		break;
	case sC:
		if (_registers->getF_C() == 1)
			_registers->addPC(read8());
		else _registers->addPC(2);
		break;
	default:
		cout << "Wrong register identifier set" << endl;
		break;
	}
	
}

// 0x22
void Instructions::LD_HLI_A()
{
	// Load A in address contained in HL
	_memory->write(_registers->getHL(), _registers->getA());
	// Increments HL
	_registers->setHL(_registers->getHL() + 1);

	_registers->addPC(1);
}

// 0x32
void Instructions::LD_HLD_A()
{
	// Load A in address contained in HL
	_memory->write(_registers->getHL(), _registers->getA());
	// Increments HL
	_registers->setHL(_registers->getHL() - 1);

	_registers->addPC(1);
}