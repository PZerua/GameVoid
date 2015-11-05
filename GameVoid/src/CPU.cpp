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

	while (run)
	{
		WORD OPCODE = _memory->read(_registers.getPC());

		switch (OPCODE)
		{
		case 0x00:	_inst->NOP(); break;
		case 0x01:	_inst->LD_n_nn(bc); break;
		case 0x02:	_inst->LD_n_A(mBC); break;
		case 0x03:	_inst->INC_nn(bc); break;
		case 0x04:	_inst->INC_n(B); break;
		case 0x05:	_inst->DEC_n(B); break;
		case 0x06:	_inst->LD_nn_n(B); break;
		case 0x07:	_inst->RLCA(); break;
		case 0x08:	_inst->LD_nn_SP(); break;
		case 0x09:	_inst->ADD_HL_n(bc); break;
		case 0x0A:	_inst->LD_A_n(mBC); break;
		case 0x0C:	_inst->INC_n(C); break;
		case 0x0D:	_inst->DEC_n(C); break;
		case 0x0E:	_inst->LD_nn_n(C); break;
		case 0x11:	_inst->LD_n_nn(de); break;
		case 0x12:	_inst->LD_n_A(mDE); break;
		case 0x13:	_inst->INC_nn(de); break;
		case 0x14:	_inst->INC_n(D); break;
		case 0x16:	_inst->LD_nn_n(D); break;
		case 0x17:	_inst->RLC(); break;
		case 0x19:	_inst->ADD_HL_n(de); break;
		case 0x1A:	_inst->LD_A_n(mDE); break;
		case 0x1C:	_inst->INC_n(E); break;
		case 0x1D:	_inst->DEC_n(E); break;
		case 0x1E:	_inst->LD_nn_n(E); break;
		case 0x21:	_inst->LD_n_nn(hl); break;
		case 0x23:	_inst->INC_nn(hl); break;
		case 0x24:	_inst->INC_n(H); break;
		case 0x25:	_inst->DEC_n(H); break;
		case 0x26:	_inst->LD_nn_n(H); break;
		case 0x29:	_inst->ADD_HL_n(hl); break;
		case 0x2C:	_inst->INC_n(L); break;
		case 0x2D:	_inst->DEC_n(L); break;
		case 0x2E:	_inst->LD_nn_n(L); break;
		case 0x31:	_inst->LD_n_nn(sp); break;
		case 0x33:	_inst->INC_nn(sp); break;
		case 0x34:	_inst->INC_n(mHL); break;
		case 0x35:	_inst->DEC_n(mHL); break;
		case 0x39:	_inst->ADD_HL_n(sp); break;
		case 0x3C:	_inst->INC_n(A); break;
		case 0x3D:	_inst->DEC_n(A); break;
		case 0x3E:	_inst->LD_A_n(n8); break;
		case 0x47:	_inst->LD_n_A(B); break;
		case 0x4F:	_inst->LD_n_A(C); break;
		case 0x57:	_inst->LD_n_A(D); break;
		case 0x5F:	_inst->LD_n_A(E); break;
		case 0x67:	_inst->LD_n_A(H); break;
		case 0x6F:	_inst->LD_n_A(L); break;
		case 0x77:	_inst->LD_n_A(mHL); break;
		case 0x78:	_inst->LD_A_n(B); break;
		case 0x79:	_inst->LD_A_n(C); break;
		case 0x7A:	_inst->LD_A_n(D); break;
		case 0x7B:	_inst->LD_A_n(E); break;
		case 0x7C:	_inst->LD_A_n(H); break;
		case 0x7D:	_inst->LD_A_n(L); break;
		case 0x7E:	_inst->LD_A_n(mHL); break;
		case 0x7F:	_inst->LD_A_n(A); break;
		case 0xEA:	_inst->LD_n_A(n16); break;
		case 0xFA:	_inst->LD_A_n(n16); break;
		default:
			cout << "Unknown or unimplemented instruction: " << hex << OPCODE << dec << endl;
			run = false;
			break;
		}
	}
}
