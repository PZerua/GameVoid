#include "Instructions.h"

Instructions::Instructions(Memory *memory, Registers *registers)
{
	_memory = memory;
	_registers = registers;
}

Instructions::~Instructions()
{

}

WORD Instructions::read16()
{
	BYTE low = _memory->read(_registers->getPC() + 1);
	BYTE high = _memory->read(_registers->getPC() + 2);
	WORD address = low + (high << 8);

	return address;
}

BYTE Instructions::read8()
{
	return _memory->read(_registers->getPC() + 1);
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
	n = read16();
	_registers->addPC(3);
}

// 0x02, 0x12, 0x47, 0x4F, 0x57, 0x5F, 0x67, 0x6F, 0x77. 0x7F, 0xEA
// n = A, B, C, D, E, H, L, (BC), (DE), (HL), (nn, inm 16)
void Instructions::LD_n_A(const regID &id)
{
	switch (id)
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
		_registers->setReg(id, _registers->getA());
		break;
	}

	_registers->addPC(1);
}
