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
	_instructions = new Instructions(_memory, &_registers);
	bool run = true;

	while (run)
	{
		WORD OPCODE = _memory->read(_registers.getPC());

		switch (OPCODE)
		{
		case 0x00:
			_instructions->NOP();
			break;
		case 0x01:
			_instructions->LD_n_nn(_registers.getBC());
			break;
		case 0x02:
			_instructions->LD_n_A(mBC);
			break;
		case 0x11:
			_instructions->LD_n_nn(_registers.getDE());
			break;
		case 0x12:
			_instructions->LD_n_A(mDE);
			break;
		case 0x21:
			_instructions->LD_n_nn(_registers.getHL());
			break;
		case 0x31:
			_instructions->LD_n_nn(_registers.getSP());
			break;
		case 0x47:
			_instructions->LD_n_A(B);
			break;
		case 0x4F:
			_instructions->LD_n_A(C);
			break;
		case 0x57:
			_instructions->LD_n_A(D);
			break;
		case 0x5F:
			_instructions->LD_n_A(E);
			break;
		case 0x67:
			_instructions->LD_n_A(H);
			break;
		case 0x6F:
			_instructions->LD_n_A(L);
			break;
		case 0x77:
			_instructions->LD_n_A(mHL);
			break;
		case 0x7F:
			_instructions->LD_n_A(A);
			break;
		case 0xEA:
			_instructions->LD_n_A(nn);
			break;
		default:
			cout << "Unknown or unimplemented instruction: " << hex << OPCODE << dec << endl;
			run = false;
			break;
		}
	}

}
