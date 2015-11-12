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

		cout << hex << "PC: " << (int)_registers.getPC() << " , OPCODE: " << OPCODE << endl;

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
		case 0x0B:	_inst->DEC_nn(bc); break;
		case 0x0C:	_inst->INC_n(C); break;
		case 0x0D:	_inst->DEC_n(C); break;
		case 0x0E:	_inst->LD_nn_n(C); break;
		case 0x0F:	_inst->RRCA(); break;
		case 0x10:	_inst->STOP(); break;
		case 0x11:	_inst->LD_n_nn(de); break;
		case 0x12:	_inst->LD_n_A(mDE); break;
		case 0x13:	_inst->INC_nn(de); break;
		case 0x14:	_inst->INC_n(D); break;
		case 0x15:	_inst->DEC_n(D); break;
		case 0x16:	_inst->LD_nn_n(D); break;
		case 0x17:	_inst->RLA(); break;
		case 0x18:	_inst->JR_n(); break;
		case 0x19:	_inst->ADD_HL_n(de); break;
		case 0x1A:	_inst->LD_A_n(mDE); break;
		case 0x1B:	_inst->DEC_nn(de); break;
		case 0x1C:	_inst->INC_n(E); break;
		case 0x1D:	_inst->DEC_n(E); break;
		case 0x1E:	_inst->LD_nn_n(E); break;
		case 0x1F:	_inst->RRA(); break;
		case 0x20:	_inst->JR_cc_n(nZ); break;
		case 0x21:	_inst->LD_n_nn(hl); break;
		case 0x22:	_inst->LD_HLI_A(); break;
		case 0x23:	_inst->INC_nn(hl); break;
		case 0x24:	_inst->INC_n(H); break;
		case 0x25:	_inst->DEC_n(H); break;
		case 0x26:	_inst->LD_nn_n(H); break;
		case 0x27:	_inst->DAA(); break;
		case 0x28:	_inst->JR_cc_n(Z); break;
		case 0x29:	_inst->ADD_HL_n(hl); break;
		case 0x2A:	_inst->LD_A_HLI(); break;
		case 0x2B:	_inst->DEC_nn(hl); break;
		case 0x2C:	_inst->INC_n(L); break;
		case 0x2D:	_inst->DEC_n(L); break;
		case 0x2E:	_inst->LD_nn_n(L); break;
		case 0x2F:	_inst->CPL(); break;
		case 0x30:	_inst->JR_cc_n(nC); break;
		case 0x31:	_inst->LD_n_nn(sp); break;
		case 0x32:	_inst->LD_HLD_A(); break;
		case 0x33:	_inst->INC_nn(sp); break;
		case 0x34:	_inst->INC_n(mHL); break;
		case 0x35:	_inst->DEC_n(mHL); break;
		case 0x36:	_inst->LD_r1_r2(mHL, n8); break;
		case 0x37:	_inst->SCF(); break;
		case 0x38:	_inst->JR_cc_n(sC); break;
		case 0x39:	_inst->ADD_HL_n(sp); break;
		case 0x3B:	_inst->DEC_nn(sp); break;
		case 0x3C:	_inst->INC_n(A); break;
		case 0x3D:	_inst->DEC_n(A); break;
		case 0x3E:	_inst->LD_A_n(n8); break;
		case 0x3F:	_inst->CCF(); break;
		case 0x40:	_inst->LD_r1_r2(B, B); break;
		case 0x41:	_inst->LD_r1_r2(B, C); break;
		case 0x42:	_inst->LD_r1_r2(B, D); break;
		case 0x43:	_inst->LD_r1_r2(B, E); break;
		case 0x44:	_inst->LD_r1_r2(B, H); break;
		case 0x45:	_inst->LD_r1_r2(B, L); break;
		case 0x46:	_inst->LD_r1_r2(B, mHL); break;
		case 0x47:	_inst->LD_n_A(B); break;
		case 0x48:	_inst->LD_r1_r2(C, B); break;
		case 0x49:	_inst->LD_r1_r2(C, C); break;
		case 0x4A:	_inst->LD_r1_r2(C, D); break;
		case 0x4B:	_inst->LD_r1_r2(C, E); break;
		case 0x4C:	_inst->LD_r1_r2(C, H); break;
		case 0x4D:	_inst->LD_r1_r2(C, L); break;
		case 0x4E:	_inst->LD_r1_r2(C, mHL); break;
		case 0x4F:	_inst->LD_n_A(C); break;
		case 0x50:	_inst->LD_r1_r2(D, B); break;
		case 0x51:	_inst->LD_r1_r2(D, C); break;
		case 0x52:	_inst->LD_r1_r2(D, D); break;
		case 0x53:	_inst->LD_r1_r2(D, E); break;
		case 0x54:	_inst->LD_r1_r2(D, H); break;
		case 0x55:	_inst->LD_r1_r2(D, L); break;
		case 0x56:	_inst->LD_r1_r2(D, mHL); break;
		case 0x57:	_inst->LD_n_A(D); break;
		case 0x58:	_inst->LD_r1_r2(E, B); break;
		case 0x59:	_inst->LD_r1_r2(E, C); break;
		case 0x5A:	_inst->LD_r1_r2(E, D); break;
		case 0x5B:	_inst->LD_r1_r2(E, E); break;
		case 0x5C:	_inst->LD_r1_r2(E, H); break;
		case 0x5D:	_inst->LD_r1_r2(E, L); break;
		case 0x5E:	_inst->LD_r1_r2(E, mHL); break;
		case 0x5F:	_inst->LD_n_A(E); break;
		case 0x60:	_inst->LD_r1_r2(H, B); break;
		case 0x61:	_inst->LD_r1_r2(H, C); break;
		case 0x62:	_inst->LD_r1_r2(H, D); break;
		case 0x63:	_inst->LD_r1_r2(H, E); break;
		case 0x64:	_inst->LD_r1_r2(H, H); break;
		case 0x65:	_inst->LD_r1_r2(H, L); break;
		case 0x66:	_inst->LD_r1_r2(H, mHL); break;
		case 0x67:	_inst->LD_n_A(H); break;
		case 0x68:	_inst->LD_r1_r2(L, B); break;
		case 0x69:	_inst->LD_r1_r2(L, C); break;
		case 0x6A:	_inst->LD_r1_r2(L, D); break;
		case 0x6B:	_inst->LD_r1_r2(L, E); break;
		case 0x6C:	_inst->LD_r1_r2(L, H); break;
		case 0x6D:	_inst->LD_r1_r2(L, L); break;
		case 0x6E:	_inst->LD_r1_r2(L, mHL); break;
		case 0x6F:	_inst->LD_n_A(L); break;
		case 0x70:	_inst->LD_r1_r2(mHL, B); break;
		case 0x71:	_inst->LD_r1_r2(mHL, C); break;
		case 0x72:	_inst->LD_r1_r2(mHL, D); break;
		case 0x73:	_inst->LD_r1_r2(mHL, E); break;
		case 0x74:	_inst->LD_r1_r2(mHL, H); break;
		case 0x75:	_inst->LD_r1_r2(mHL, L); break;
		case 0x76:	_inst->HALT(); break;
		case 0x77:	_inst->LD_n_A(mHL); break;
		case 0x78:	_inst->LD_A_n(B); break;
		case 0x79:	_inst->LD_A_n(C); break;
		case 0x7A:	_inst->LD_A_n(D); break;
		case 0x7B:	_inst->LD_A_n(E); break;
		case 0x7C:	_inst->LD_A_n(H); break;
		case 0x7D:	_inst->LD_A_n(L); break;
		case 0x7E:	_inst->LD_A_n(mHL); break;
		case 0x7F:	_inst->LD_A_n(A); break;
		case 0x80:	_inst->ADD_A_n(B); break;
		case 0x81:	_inst->ADD_A_n(C); break;
		case 0x82:	_inst->ADD_A_n(D); break;
		case 0x83:	_inst->ADD_A_n(E); break;
		case 0x84:	_inst->ADD_A_n(H); break;
		case 0x85:	_inst->ADD_A_n(L); break;
		case 0x86:	_inst->ADD_A_n(mHL); break;
		case 0x87:	_inst->ADD_A_n(A); break;
		case 0x88:	_inst->ADC_A_n(B); break;
		case 0x89:	_inst->ADC_A_n(C); break;
		case 0x8A:	_inst->ADC_A_n(D); break;
		case 0x8B:	_inst->ADC_A_n(E); break;
		case 0x8C:	_inst->ADC_A_n(H); break;
		case 0x8D:	_inst->ADC_A_n(L); break;
		case 0x8E:	_inst->ADC_A_n(mHL); break;
		case 0x8F:	_inst->ADC_A_n(A); break;
		case 0x90:	_inst->SUB_A_n(B); break;
		case 0x91:	_inst->SUB_A_n(C); break;
		case 0x92:	_inst->SUB_A_n(D); break;
		case 0x93:	_inst->SUB_A_n(E); break;
		case 0x94:	_inst->SUB_A_n(H); break;
		case 0x95:	_inst->SUB_A_n(L); break;
		case 0x96:	_inst->SUB_A_n(mHL); break;
		case 0x97:	_inst->SUB_A_n(A); break;
		case 0x98:	_inst->SBC_A_n(B); break;
		case 0x99:	_inst->SBC_A_n(C); break;
		case 0x9A:	_inst->SBC_A_n(D); break;	
		case 0x9B:	_inst->SBC_A_n(E); break;
		case 0x9C:	_inst->SBC_A_n(H); break;
		case 0x9D:	_inst->SBC_A_n(L); break;
		case 0x9E:	_inst->SBC_A_n(mHL); break;
		case 0x9F:	_inst->SBC_A_n(A); break;
		case 0xA0:	_inst->AND_n(B); break;
		case 0xA1:	_inst->AND_n(C); break;
		case 0xA2:	_inst->AND_n(D); break;
		case 0xA3:	_inst->AND_n(E); break;
		case 0xA4:	_inst->AND_n(H); break;
		case 0xA5:	_inst->AND_n(L); break;
		case 0xA6:	_inst->AND_n(mHL); break;
		case 0xA7:	_inst->AND_n(A); break;
		case 0xA8:	_inst->XOR_n(B); break;
		case 0xA9:	_inst->XOR_n(C); break;
		case 0xAA:	_inst->XOR_n(D); break;
		case 0xAB:	_inst->XOR_n(E); break;
		case 0xAC:	_inst->XOR_n(H); break;
		case 0xAD:	_inst->XOR_n(L); break;
		case 0xAE:	_inst->XOR_n(mHL); break;
		case 0xAF:	_inst->XOR_n(A); break;
		case 0xB0:	_inst->OR_n(B); break;
		case 0xB1:	_inst->OR_n(C); break;
		case 0xB2:	_inst->OR_n(D); break;
		case 0xB3:	_inst->OR_n(E); break;
		case 0xB4:	_inst->OR_n(H); break;
		case 0xB5:	_inst->OR_n(L); break;
		case 0xB6:	_inst->OR_n(mHL); break;
		case 0xB7:	_inst->OR_n(A); break;
		case 0xB8:	_inst->CP_n(B); break;
		case 0xB9:	_inst->CP_n(C); break;
		case 0xBA:	_inst->CP_n(D); break;
		case 0xBB:	_inst->CP_n(E); break;
		case 0xBC:	_inst->CP_n(H); break;
		case 0xBD:	_inst->CP_n(L); break;
		case 0xBE:	_inst->CP_n(mHL); break;
		case 0xBF:	_inst->CP_n(A); break;
		case 0xC0:	_inst->RET_cc(nZ); break;
		case 0xC1:	_inst->POP_nn(bc); break;
		case 0xC2:	_inst->JP_cc_nn(nZ); break;
		case 0xC3:	_inst->JP_nn(); break;
		case 0xC4:	_inst->CALL_cc_nn(nZ); break;
		case 0xC6:	_inst->ADD_A_n(n8); break;
		case 0xC8:	_inst->RET_cc(Z); break;
		case 0xC9:	_inst->RET(); break;
		case 0xCA:	_inst->JP_cc_nn(Z); break;
		case 0xCC:	_inst->CALL_cc_nn(Z); break;
		case 0xCD:	_inst->CALL_nn(); break;
		case 0xCE:	_inst->ADC_A_n(n8); break;
		case 0xD0:	_inst->RET_cc(nC); break;
		case 0xD1:	_inst->POP_nn(de); break;
		case 0xD2:	_inst->JP_cc_nn(nC); break;
		case 0xD4:	_inst->CALL_cc_nn(nC); break;
		case 0xD6:	_inst->SUB_A_n(n8); break;
		case 0xD8:	_inst->RET_cc(sC); break;
		case 0xDA:	_inst->JP_cc_nn(sC); break;
		case 0xDC:	_inst->CALL_cc_nn(sC); break;
		case 0xDE:	_inst->SBC_A_n(n8); break;
		case 0xE0:	_inst->LDH_n_A(); break;
		case 0xE1:	_inst->POP_nn(hl); break;
		case 0xEA:	_inst->LD_n_A(n16); break;
		case 0xEE:	_inst->XOR_n(n8); break;
		case 0xF0:	_inst->LDH_A_n(); break;
		case 0xF1:	_inst->POP_nn(af); break;
		case 0xF3:	_inst->DI(); break;
		case 0xF6:	_inst->OR_n(n8); break;
		case 0xFA:	_inst->LD_A_n(n16); break;
		case 0xFB:	_inst->EI(); break;
		case 0xFE:	_inst->CP_n(n8); break;
		default:
			cout << hex << "Unknown or unimplemented instruction \""<< OPCODE << "\" at PC = " 
				<< _registers.getPC() << dec << endl;
			run = false;
			break;
		}
	}
}
