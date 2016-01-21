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
		return true;
	else return false;
}

bool Instructions::hasCarry8(const int &a, const int &b)
{
	if ((((a & 0x00FF) + (b & 0x00FF)) & 0x100) == 0x100)
		return true;
	else return false;
}

bool Instructions::hasHalfCarry16(const int &a, const int &b)
{
	if ((((a & 0x0FFF) + (b & 0x0FFF)) & 0x1000) == 0x1000)
		return true;
	else return false;
}

bool Instructions::hasCarry16(const int &a, const int &b)
{
	if ((((a & 0xFFFF) + (b & 0xFFFF)) & 0x10000) == 0x10000)
		return true;
	else return false;
}

bool Instructions::hasHalfBorrow8(const int &a, const int &b)
{
	if ((((a & 0x000F) - (b & 0x000F)) & 0x10) == 0x10)
		return true;
	else return false;
}

bool Instructions::hasBorrow8(const int &a, const int &b)
{
	if ((((a & 0x00FF) - (b & 0x00FF)) & 0x100) == 0x100)
		return true;
	else return false;
}

bool Instructions::hasHalfBorrow16(const int &a, const int &b)
{
	if ((((a & 0x0FFF) - (b & 0x0FFF)) & 0x1000) == 0x1000)
		return true;
	else return false;
}

bool Instructions::hasBorrow16(const int &a, const int &b)
{
	if ((((a & 0xFFFF) - (b & 0xFFFF)) & 0x10000) == 0x10000)
		return true;
	else return false;
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

// 0x02, 0x12, 0x47, 0x4F, 0x57, 0x5F, 0x67, 0x6F, 0x77, 0x7F, 0xEA
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

	if (n != n16)
		_registers->addPC(1);
	else _registers->addPC(3);
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
	switch (n)
	{
	case mHL:
		_registers->setF_H(hasHalfCarry8(_memory->read(_registers->getHL()), 1));
		_memory->write(_registers->getHL(), _memory->read(_registers->getHL()) + 1);
		break;
	default:
		// Set carry in flag
		_registers->setF_H(hasHalfCarry8(_registers->getReg(n), 1));
		_registers->setReg(n, _registers->getReg(n) + 1);
		break;
	}

	// Set Z and N in flag
	_registers->setF_Z(_registers->getReg(n) == 0);
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
		_registers->setReg(n, _registers->getReg(n) - 1);
		break;
	}

	// Set Z and N in flag
	_registers->setF_Z(_registers->getReg(n) == 0);
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
	_registers->setF_Z(0);

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

	if (n == n8)
		_registers->addPC(2);
	else if (n != n16)
		_registers->addPC(1);
	else _registers->addPC(3);
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
	_registers->setF_Z(0);

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
	_registers->setF_Z(0);

	_registers->addPC(1);
}

// 0x18
void Instructions::JR_n()
{
	// Signed value
	char address;

	address = read8();

	_registers->addPC(2 + address);
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
	_registers->setF_Z(0);

	_registers->addPC(1);
}

// 0x20, 0x28, 0x30, 0x38
void Instructions::JR_cc_n(const regID &n, bool &condTaken)
{
	switch (n)
	{
	case nZ:
		if (_registers->getF_Z() == 0)
		{
			JR_n();
			condTaken = true;
		}
		else
		{
			_registers->addPC(2);
			condTaken = false;
		}
		break;
	case Z:
		if (_registers->getF_Z() == 1)
		{
			JR_n();
			condTaken = true;
		}
		else
		{
			_registers->addPC(2);
			condTaken = false;
		}
		break;
	case nC:
		if (_registers->getF_C() == 0)
		{
			JR_n();
			condTaken = true;
		}
		else
		{
			_registers->addPC(2);
			condTaken = false;
		}
		break;
	case sC:
		if (_registers->getF_C() == 1)
		{
			JR_n();
			condTaken = true;
		}
		else
		{
			_registers->addPC(2);
			condTaken = false;
		}
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

// 0x27
void Instructions::DAA()
{
	// Least significant bits
	BYTE LSB;
	// Most significant bits
	BYTE MSB;

	if (!_registers->getF_N())
	{
		LSB = _registers->getA() & 0x0F;
		if (LSB > 0x09 || _registers->getF_H())
		{
			_registers->setA(_registers->getA() + 0x06);
		}
		MSB = (_registers->getA() & 0xF0) >> 4;
		if (MSB > 0x09 || _registers->getF_C())
		{
			_registers->setA(_registers->getA() + 0x60);
			_registers->setF_C(1);
		}
		else _registers->setF_C(0);
	}
	else
	{
		LSB = _registers->getA() & 0x0F;
		if (LSB > 0x09 || _registers->getF_H())
		{
			_registers->setA(_registers->getA() + 0x06);
		}
		MSB = (_registers->getA() & 0xF0) >> 4;
		if (MSB > 0x09 || _registers->getF_C())
		{
			_registers->setA(_registers->getA() + 0x60);
			_registers->setF_C(1);
		}
		else _registers->setF_C(0);
	}

	_registers->setF_Z(_registers->getA() == 0);
	_registers->setF_H(0);

	_registers->addPC(1);
}

// 0x2A
void Instructions::LD_A_HLI()
{
	// Loads HL address value into A
	_registers->setA(_memory->read(_registers->getHL()));
	// Increment HL
	_registers->setHL(_registers->getHL() + 1);

	_registers->addPC(1);
}

// 0x2F
void Instructions::CPL()
{
	_registers->setA(~_registers->getA());
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

// 0x36, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x48, 0x49, 0x4A, 0x4B
// 0x4C, 0x4D, 0x4E, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x58, 0x59
// 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66
// 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x70, 0x71, 0x72, 0x73, 0x74
// 0x75, 0x76, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E
void Instructions::LD_r1_r2(const regID &r1, const regID &r2)
{
	if (r1 == mHL)
	{
		if (r2 != n8)
			_memory->write(_registers->getHL(), (BYTE)_registers->getReg(r2));
		else _memory->write(_registers->getHL(), read8());
		_registers->addPC(2);
	}
	else
	{
		if (r2 != mHL)
			_registers->setReg(r1, _registers->getReg(r2));
		else _registers->setReg(r1, _memory->read(_registers->getReg(r2)));
		_registers->addPC(1);
	}
}

// 0x37
void Instructions::SCF()
{
	_registers->setF_N(0);
	_registers->setF_H(0);
	_registers->setF_C(1);
	_registers->addPC(1);
}

// 0x3F
void Instructions::CCF()
{
	_registers->setF_N(0);
	_registers->setF_H(0);
	_registers->setF_C(!_registers->getF_C());
	_registers->addPC(1);
}

// 0x76
void Instructions::HALT()
{
	// TODO
	_registers->addPC(1);
}

// 0x80, 0x81, 0x82, 0x83, 0x84, 0x85 0x86, 0x87, 0xC6
void Instructions::ADD_A_n(const regID &n)
{
	switch (n)
	{
	case mHL:
		_registers->setF_H(hasHalfCarry8(_registers->getA(), _memory->read(_registers->getHL())));
		_registers->setF_C(hasCarry8(_registers->getA(), _memory->read(_registers->getHL())));
		_registers->setA(_registers->getA() + _memory->read(_registers->getHL()));
		break;
	case n8:
		_registers->setF_H(hasHalfCarry8(_registers->getA(), read8()));
		_registers->setF_C(hasCarry8(_registers->getA(), read8()));
		_registers->setA(_registers->getA() + read8());
		break;
	default:
		_registers->setF_H(hasHalfCarry8(_registers->getA(), _registers->getReg(n)));
		_registers->setF_C(hasCarry8(_registers->getA(), _registers->getReg(n)));
		_registers->setA(_registers->getA() + _registers->getReg(n));
		break;
	}
	// Set flags
	_registers->setF_Z(_registers->getA() == 0);
	_registers->setF_N(0);
	
	if (n != n8)
		_registers->addPC(1);
	else _registers->addPC(2);
}

// 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0xCE
void Instructions::ADC_A_n(const regID &n)
{
	switch (n)
	{
	case mHL:
		_registers->setF_H(hasHalfCarry8(_registers->getA(), _memory->read(_registers->getHL() + _registers->getF_C())));
		_registers->setF_C(hasCarry8(_registers->getA(), _memory->read(_registers->getHL() + _registers->getF_C())));
		_registers->setA(_registers->getA() + _memory->read(_registers->getHL()) + _registers->getF_C());
		break;
	case n8:
		_registers->setF_H(hasHalfCarry8(_registers->getA(), read8() + _registers->getF_C()));
		_registers->setF_C(hasCarry8(_registers->getA(), read8() + _registers->getF_C()));
		_registers->setA(_registers->getA() + read8() + _registers->getF_C());
		break;
	default:
		_registers->setF_H(hasHalfCarry8(_registers->getA(), _registers->getReg(n)));
		_registers->setF_C(hasCarry8(_registers->getA(), _registers->getReg(n)));
		_registers->setA(_registers->getA() + _registers->getReg(n) + _registers->getF_C());
		break;
	}
	// Set flags
	_registers->setF_Z(_registers->getA() == 0);
	_registers->setF_N(0);

	if (n != n8)
		_registers->addPC(1);
	else _registers->addPC(2);
}

// 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0xD6
void Instructions::SUB_A_n(const regID &n)
{
	switch (n)
	{
	case mHL:
		_registers->setF_H(hasHalfBorrow8(_registers->getA(), _memory->read(_registers->getHL())));
		_registers->setF_C(hasBorrow8(_registers->getA(), _memory->read(_registers->getHL())));
		_registers->setA(_registers->getA() - _memory->read(_registers->getHL()));
		break;
	case n8:
		_registers->setF_H(hasHalfBorrow8(_registers->getA(), read8()));
		_registers->setF_C(hasBorrow8(_registers->getA(), read8()));
		_registers->setA(_registers->getA() - read8());
		break;
	default:
		_registers->setF_H(hasHalfBorrow8(_registers->getA(), _registers->getReg(n)));
		_registers->setF_C(hasBorrow8(_registers->getA(), _registers->getReg(n)));
		_registers->setA(_registers->getA() - _registers->getReg(n));
		break;
	}
	// Set flags
	_registers->setF_Z(_registers->getA() == 0);
	_registers->setF_N(1);

	if (n != n8)
		_registers->addPC(1);
	else _registers->addPC(2);
}

// 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xDE
void Instructions::SBC_A_n(const regID &n)
{
	switch (n)
	{
	case mHL:
		_registers->setF_H(hasHalfBorrow8(_registers->getA(), _memory->read(_registers->getHL() + _registers->getF_C())));
		_registers->setF_C(hasBorrow8(_registers->getA(), _memory->read(_registers->getHL() + _registers->getF_C())));
		_registers->setA(_registers->getA() - (_memory->read(_registers->getHL()) + _registers->getF_C()));
		break;
	case n8:
		_registers->setF_H(hasHalfBorrow8(_registers->getA(), read8() + _registers->getF_C()));
		_registers->setF_C(hasBorrow8(_registers->getA(), read8() + _registers->getF_C()));
		_registers->setA(_registers->getA() - (read8() + _registers->getF_C()));
		break;
	default:
		_registers->setF_H(hasHalfBorrow8(_registers->getA(), _registers->getReg(n)));
		_registers->setF_C(hasBorrow8(_registers->getA(), _registers->getReg(n)));
		_registers->setA(_registers->getA() - (_registers->getReg(n) + _registers->getF_C()));
		break;
	}
	// Set flags
	_registers->setF_Z(_registers->getA() == 0);
	_registers->setF_N(1);

	if (n != n8)
		_registers->addPC(1);
	else _registers->addPC(2);
}

// 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xE6
void Instructions::AND_n(const regID &n)
{
	switch (n)
	{
	case mHL:
		_registers->setA(_memory->read(_registers->getHL()) & _registers->getA());
		break;
	case n8:
		_registers->setA(read8() & _registers->getA());
		break;
	default:
		_registers->setA(_registers->getReg(n) & _registers->getA());
		break;
	}
	// Set Flags
	_registers->setF_N(0);
	_registers->setF_C(0);
	_registers->setF_H(1);
	_registers->setF_Z(_registers->getA() == 0);

	if (n != n8)
		_registers->addPC(1);
	else _registers->addPC(2);
}

// 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xEE
void Instructions::XOR_n(const regID &n)
{
	switch (n)
	{
	case mHL:
		_registers->setA(_memory->read(_registers->getHL()) ^ _registers->getA());
		break;
	case n8:
		_registers->setA(read8() ^ _registers->getA());
		break;
	default:
		_registers->setA(_registers->getReg(n) ^ _registers->getA());
		break;
	}
	// Set Flags
	_registers->setF_N(0);
	_registers->setF_C(0);
	_registers->setF_H(0);
	_registers->setF_Z(_registers->getA() == 0);

	if (n != n8)
		_registers->addPC(1);
	else _registers->addPC(2);
}

// 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xF6
void Instructions::OR_n(const regID &n)
{
	switch (n)
	{
	case mHL:
		_registers->setA(_memory->read(_registers->getHL()) | _registers->getA());
		break;
	case n8:
		_registers->setA(read8() | _registers->getA());
		break;
	default:
		_registers->setA(_registers->getReg(n) | _registers->getA());
		break;
	}
	// Set Flags
	_registers->setF_N(0);
	_registers->setF_C(0);
	_registers->setF_H(0);
	_registers->setF_Z(_registers->getA() == 0);

	if (n != n8)
		_registers->addPC(1);
	else _registers->addPC(2);
}

// 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xFE
void Instructions::CP_n(const regID &n)
{
	switch (n)
	{
	case mHL:
		_registers->setF_H(hasHalfBorrow8(_registers->getA(), _memory->read(_registers->getHL())));
		_registers->setF_C(hasBorrow8(_registers->getA(), _memory->read(_registers->getHL())));
		_registers->setF_Z((_registers->getA() - _memory->read(_registers->getHL())) == 0);
		break;
	case n8:
		_registers->setF_H(hasHalfBorrow8(_registers->getA(), read8()));
		_registers->setF_C(hasBorrow8(_registers->getA(), read8()));
		_registers->setF_Z((_registers->getA() - read8()) == 0);
		break;
	default:
		_registers->setF_H(hasHalfBorrow8(_registers->getA(), _registers->getReg(n)));
		_registers->setF_C(hasBorrow8(_registers->getA(), _registers->getReg(n)));
		_registers->setF_Z((_registers->getA() - _registers->getReg(n)) == 0);
		break;
	}
	// Set flags
	_registers->setF_N(1);

	if (n != n8)
		_registers->addPC(1);
	else _registers->addPC(2);
}

// 0xC0, 0xC8, 0xD0, 0xD8
void Instructions::RET_cc(const regID &cc, bool &condTaken)
{
	switch (cc)
	{
	case nZ:
		if (_registers->getF_Z() == 0)
		{
			_registers->setPC(_memory->read(_registers->getSP()));
			_registers->addSP(1);
			_registers->setPC((_registers->getPC() << 8) + _memory->read(_registers->getSP()));
			_registers->addSP(1);
			condTaken = true;
		}
		else
		{
			_registers->addPC(1);
			condTaken = false;
		}
		break;
	case Z:
		if (_registers->getF_Z() == 1)
		{
			_registers->setPC(_memory->read(_registers->getSP()));
			_registers->addSP(1);
			_registers->setPC((_registers->getPC() << 8) + _memory->read(_registers->getSP()));
			_registers->addSP(1);
			condTaken = true;
		}
		else
		{
			_registers->addPC(1);
			condTaken = false;
		}
		break;
	case nC:
		if (_registers->getF_C() == 0)
		{
			_registers->setPC(_memory->read(_registers->getSP()));
			_registers->addSP(1);
			_registers->setPC((_registers->getPC() << 8) + _memory->read(_registers->getSP()));
			_registers->addSP(1);
			condTaken = true;
		}
		else 
		{
			_registers->addPC(1);
			condTaken = false;
		}
		break;
	case sC:
		if (_registers->getF_C() == 1)
		{
			_registers->setPC(_memory->read(_registers->getSP()));
			_registers->addSP(1);
			_registers->setPC((_registers->getPC() << 8) + _memory->read(_registers->getSP()));
			_registers->addSP(1);
			condTaken = true;
		}
		else 
		{
			_registers->addPC(1);
			condTaken = false;
		}
		break;
	default:
		cout << "Wrong register identifier set" << endl;
		break;
	}
}

// 0xC1, 0xD1, 0xE1, 0xF1
void Instructions::POP_nn(const regID &nn)
{
	switch (nn)
	{
	case bc:
		_registers->setBC(_memory->read(_registers->getSP()));
		_registers->addSP(1);
		_registers->setBC((_registers->getBC() << 8) + _memory->read(_registers->getSP()));
		_registers->addSP(1);
		break;
	case de:
		_registers->setDE(_memory->read(_registers->getSP()));
		_registers->addSP(1);
		_registers->setDE((_registers->getDE() << 8) + _memory->read(_registers->getSP()));
		_registers->addSP(1);
		break;
	case hl:
		_registers->setHL(_memory->read(_registers->getSP()));
		_registers->addSP(1);
		_registers->setHL((_registers->getHL() << 8) + _memory->read(_registers->getSP()));
		_registers->addSP(1);
		break;
	case af:
		_registers->setAF(_memory->read(_registers->getSP()));
		_registers->addSP(1);
		_registers->setAF((_registers->getAF() << 8) + _memory->read(_registers->getSP()));
		_registers->addSP(1);
		break;
	default:
		cout << "Wrong register ID" << endl;
		break;
	}

	_registers->addPC(1);
}

// 0xC3
void Instructions::JP_nn()
{
	_registers->setPC(read16());
}

// 0xC2, 0xCA, 0xD2, 0xDA
void Instructions::JP_cc_nn(const regID &cc, bool &condTaken)
{
	switch (cc)
	{
	case nZ:
		if (_registers->getF_Z() == 0)
		{
			_registers->setPC(read16());
			condTaken = true;
		}
		else
		{
			_registers->addPC(3);
			condTaken = false;
		}
		break;
	case Z:
		if (_registers->getF_Z() == 1)
		{
			_registers->setPC(read16());
			condTaken = true;
		}
		else
		{
			_registers->addPC(3);
			condTaken = false;
		}
		break;
	case nC:
		if (_registers->getF_C() == 0)
		{
			_registers->setPC(read16());
			condTaken = true;
		}
		else
		{
			_registers->addPC(3);
			condTaken = false;
		}
		break;
	case sC:
		if (_registers->getF_C() == 1)
		{
			_registers->setPC(read16());
			condTaken = true;
		}
		else
		{
			_registers->addPC(3);
			condTaken = false;
		}
		break;
	default:
		cout << "Wrong register identifier set" << endl;
		break;
	}
}

// 0xC9
void Instructions::RET()
{
	_registers->setPC(_memory->read(_registers->getSP()));
	_registers->addSP(1);
	_registers->setPC((_registers->getPC() << 8) + _memory->read(_registers->getSP()));
	_registers->addSP(1);
}

// 0xC4, 0xCC, 0xD4, 0xDC
void Instructions::CALL_cc_nn(const regID &cc, bool &condTaken)
{
	switch (cc)
	{
	case nZ:
		if (_registers->getF_Z() == 0)
		{
			CALL_nn();
			condTaken = true;
		}
		else
		{
			_registers->addPC(3);
			condTaken = false;
		}
		break;
	case Z:
		if (_registers->getF_Z() == 1)
		{
			CALL_nn();
			condTaken = true;
		}
		else
		{
			_registers->addPC(3);
			condTaken = false;
		}
		break;
	case nC:
		if (_registers->getF_C() == 0)
		{
			CALL_nn();
			condTaken = true;
		}
		else
		{
			_registers->addPC(3);
			condTaken = false;
		}
		break;
	case sC:
		if (_registers->getF_C() == 1)
		{
			CALL_nn();
			condTaken = true;
		}
		else
		{
			_registers->addPC(3);
			condTaken = false;
		}
		break;
	default:
		cout << "Wrong register identifier set" << endl;
		break;
	}
}

// 0xC5, 0xD5, 0xE5, 0xF5
void Instructions::PUSH_nn(const regID &nn)
{
	_registers->setSP(_registers->getSP() - 1);
	_memory->write(_registers->getSP(), (_registers->getReg(nn) & 0xFF00) >> 8);
	_registers->setSP(_registers->getSP() - 1);
	_memory->write(_registers->getSP(), _registers->getReg(nn) & 0x00FF);
	_registers->addPC(1);
}

// 0xC7, 0xCF, 0xD7, 0xDF, 0xE7, 0xEF, 0xF7, 0xFF
void Instructions::RST_n(const BYTE &n)
{
	_registers->addPC(1);
	_registers->setSP(_registers->getSP() - 1);
	_memory->write(_registers->getSP(), (_registers->getPC() & 0xFF00) >> 8);
	_registers->setSP(_registers->getSP() - 1);
	_memory->write(_registers->getSP(), _registers->getPC() & 0x00FF);
	_registers->setPC(0x0000 + n);
}

// 0xCD
void Instructions::CALL_nn()
{
	_memory->write(_registers->getSP() - 1, ((_registers->getPC() + 3) & 0xFF00) >> 8);
	_memory->write(_registers->getSP() - 2, (_registers->getPC() + 3) & 0x00FF);
	_registers->setSP(_registers->getSP() - 2);
	_registers->setPC(read16());
}

// 0xE0
void Instructions::LDH_n_A()
{
	_memory->write(0xFF00 + read8(), _registers->getA());
	_registers->addPC(2);
}

// 0xE2
void Instructions::LD_C_A()
{
	_memory->write(0xFF00 + _registers->getC(), _registers->getA());
	_registers->addPC(1);
}

// 0xE8
void Instructions::ADD_SP_n()
{
	char signedNum = read8();

	_registers->setF_Z(0);
	_registers->setF_N(0);
	_registers->setF_H(hasHalfCarry16(_registers->getSP(), signedNum));
	_registers->setF_C(hasCarry16(_registers->getSP(), signedNum));
	_registers->setSP(_registers->getSP() + signedNum);

	_registers->addPC(2);
}

// 0xE9
void Instructions::JP_HL()
{
	_registers->setPC(_registers->getHL());
}

// 0xF0
void Instructions::LDH_A_n()
{
	_registers->setA(_memory->read(0xFF00 + read8()));
	_registers->addPC(2);
}

// 0xF2
void Instructions::LD_A_C()
{
	_registers->setA(_memory->read(0xFF00 + _registers->getC()));
	_registers->addPC(1);
}

// 0xF3
void Instructions::DI(bool &IME)
{
	IME = false;
	_registers->addPC(1);
}

// 0xFB
void Instructions::EI(bool &IME)
{
	IME = true;
	_registers->addPC(1);
}

Registers *Instructions::getRegisters()
{
	return _registers;
}
