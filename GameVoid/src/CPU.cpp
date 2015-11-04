#include "CPU.h"
 
CPU::CPU(Memory *memory)
{
	_memory = memory;
}

CPU::~CPU()
{

}

void CPU::run()
{
	_inst = new Instructions(_memory, &_registers);
	bool run = true;

	int temp = 0xFF + 0xFF;

	cout << _inst->hasCarry8(0xFF, 0xFF) << endl;

	while (run)
	{
		WORD OPCODE = _memory->read(_registers.getPC());

		switch (OPCODE)
		{
		case 0x00:	_inst->NOP(); break;
		case 0x01:	_inst->LD_n_nn(_registers.getBC()); break;
		case 0x02:	_inst->LD_n_A(mBC); break;
		case 0x03:	_inst->INC_nn(_registers.getBC()); break;
		case 0x04:	_inst->INC_n(B); break;
		case 0x0C:	_inst->INC_n(C); break;
		case 0x11:	_inst->LD_n_nn(_registers.getDE()); break;
		case 0x12:	_inst->LD_n_A(mDE); break;
		case 0x13:	_inst->INC_nn(_registers.getDE()); break;
		case 0x14:	_inst->INC_n(D); break;
		case 0x1C:	_inst->INC_n(E); break;
		case 0x21:	_inst->LD_n_nn(_registers.getHL()); break;
		case 0x23:	_inst->INC_nn(_registers.getHL()); break;
		case 0x24:	_inst->INC_n(H); break;
		case 0x2C:	_inst->INC_n(L); break;
		case 0x31:	_inst->LD_n_nn(_registers.getSP()); break;
		case 0x33:	_inst->INC_nn(_registers.getSP()); break;
		case 0x34:	_inst->INC_n(mHL); break;
		case 0x3C:	_inst->INC_n(A); break;
		case 0x47:	_inst->LD_n_A(B); break;
		case 0x4F:	_inst->LD_n_A(C); break;
		case 0x57:	_inst->LD_n_A(D); break;
		case 0x5F:	_inst->LD_n_A(E); break;
		case 0x67:	_inst->LD_n_A(H); break;
		case 0x6F:	_inst->LD_n_A(L); break;
		case 0x77:	_inst->LD_n_A(mHL); break;
		case 0x7F:	_inst->LD_n_A(A); break;
		case 0xEA:	_inst->LD_n_A(nn); break;
		default:
			cout << "Unknown or unimplemented instruction: " << hex << OPCODE << dec << endl;
			run = false;
			break;
		}
	}

}
