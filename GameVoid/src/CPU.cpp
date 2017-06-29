#include "CPU.h"
 
void CPU::init(Memory *memory)
{
	_memory = memory;
	_inst = new Instructions(_memory, &_registers);
	_timeCounter = 1024;
	_divideCounter = 0;
	initCyclesArrays();
	IME = true;
}

int CPU::fetch()
{
	_condTaken = false;
	int CBcycles = 0;
	static bool debug = false;
	static int count = 0;

	BYTE OPCODE = _memory->read(_registers.getPC());

	/*if (OPCODE == 0xE0)
	{
		cout << "DEBUG" << endl;
		debug = false;
	}*/
	
	lastPC = _registers.getPC();
	lastOPCODE = OPCODE;

	/*if (_registers.getPC() == 0x0D16)
	{
		cout << "DEBUGING TIME" << endl;
		debug = false;
	}*/

	if (debug) 
	{
		cout << hex << "PC: " << (int)_registers.getPC() << " | OPCODE: " << (int)OPCODE << "\t";
		cout << hex << "AF: " << (int)_registers.getAF() << " | BC: " << (int)_registers.getBC() << " | DE: " << (int)_registers.getDE()
			<< " | HL: " << (int)_registers.getHL() << " | SP: " << (int)_registers.getSP() << endl;
		count++;
	}

	if ((int)_registers.getPC() == 0x381)
		std::cout << "Here" << std::endl;
	
	if (!_registers.haltEnabled())
	{
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
		case 0x20:	_inst->JR_cc_n(nZ, _condTaken); break;
		case 0x21:	_inst->LD_n_nn(hl); break;
		case 0x22:	_inst->LD_HLI_A(); break;
		case 0x23:	_inst->INC_nn(hl); break;
		case 0x24:	_inst->INC_n(H); break;
		case 0x25:	_inst->DEC_n(H); break;
		case 0x26:	_inst->LD_nn_n(H); break;
		case 0x27:	_inst->DAA(); break;
		case 0x28:	_inst->JR_cc_n(Z, _condTaken); break;
		case 0x29:	_inst->ADD_HL_n(hl); break;
		case 0x2A:	_inst->LD_A_HLI(); break;
		case 0x2B:	_inst->DEC_nn(hl); break;
		case 0x2C:	_inst->INC_n(L); break;
		case 0x2D:	_inst->DEC_n(L); break;
		case 0x2E:	_inst->LD_nn_n(L); break;
		case 0x2F:	_inst->CPL(); break;
		case 0x30:	_inst->JR_cc_n(nC, _condTaken); break;
		case 0x31:	_inst->LD_n_nn(sp); break;
		case 0x32:	_inst->LD_HLD_A(); break;
		case 0x33:	_inst->INC_nn(sp); break;
		case 0x34:	_inst->INC_n(mHL); break;
		case 0x35:	_inst->DEC_n(mHL); break;
		case 0x36:	_inst->LD_r1_r2(mHL, n8); break;
		case 0x37:	_inst->SCF(); break;
		case 0x38:	_inst->JR_cc_n(sC, _condTaken); break;
		case 0x39:	_inst->ADD_HL_n(sp); break;
		case 0x3A:	_inst->LD_A_HLD(); break;
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
		case 0xC0:	_inst->RET_cc(nZ, _condTaken); break;
		case 0xC1:	_inst->POP_nn(bc); break;
		case 0xC2:	_inst->JP_cc_nn(nZ, _condTaken); break;
		case 0xC3:	_inst->JP_nn(); break;
		case 0xC4:	_inst->CALL_cc_nn(nZ, _condTaken); break;
		case 0xC5:	_inst->PUSH_nn(bc); break;
		case 0xC6:	_inst->ADD_A_n(n8); break;
		case 0xC7:	_inst->RST_n(0x00); break;
		case 0xC8:	_inst->RET_cc(Z, _condTaken); break;
		case 0xC9:	_inst->RET(); break;
		case 0xCA:	_inst->JP_cc_nn(Z, _condTaken); break;
		case 0xCB:	CBcycles = CB_prefix(_inst); break;
		case 0xCC:	_inst->CALL_cc_nn(Z, _condTaken); break;
		case 0xCD:	_inst->CALL_nn(); break;
		case 0xCE:	_inst->ADC_A_n(n8); break;
		case 0xCF:	_inst->RST_n(0x08); break;
		case 0xD0:	_inst->RET_cc(nC, _condTaken); break;
		case 0xD1:	_inst->POP_nn(de); break;
		case 0xD2:	_inst->JP_cc_nn(nC, _condTaken); break;
		case 0xD4:	_inst->CALL_cc_nn(nC, _condTaken); break;
		case 0xD5:	_inst->PUSH_nn(de); break;
		case 0xD6:	_inst->SUB_A_n(n8); break;
		case 0xD7:	_inst->RST_n(0x10); break;
		case 0xD8:	_inst->RET_cc(sC, _condTaken); break;
		case 0xD9:	_inst->RETI(IME); break;
		case 0xDA:	_inst->JP_cc_nn(sC, _condTaken); break;
		case 0xDC:	_inst->CALL_cc_nn(sC, _condTaken); break;
		case 0xDE:	_inst->SBC_A_n(n8); break;
		case 0xDF:	_inst->RST_n(0x18); break;
		case 0xE0:	_inst->LDH_n_A(); break;
		case 0xE1:	_inst->POP_nn(hl); break;
		case 0xE2:  _inst->LD_C_A(); break;
		case 0xE5:	_inst->PUSH_nn(hl); break;
		case 0xE6:	_inst->AND_n(n8); break;
		case 0xE7:	_inst->RST_n(0x20); break;
		case 0xE8:	_inst->ADD_SP_n(); break;
		case 0xE9:	_inst->JP_HL(); break;
		case 0xEA:	_inst->LD_n_A(n16); break;
		case 0xEE:	_inst->XOR_n(n8); break;
		case 0xEF:	_inst->RST_n(0x28); break;
		case 0xF0:	_inst->LDH_A_n(); break;
		case 0xF1:	_inst->POP_nn(af); break;
		case 0xF2:	_inst->LD_A_C(); break;
		case 0xF3:	_inst->DI(IME); break;
		case 0xF5:	_inst->PUSH_nn(af); break;
		case 0xF6:	_inst->OR_n(n8); break;
		case 0xF7:	_inst->RST_n(0x30); break;
		case 0xF8:	_inst->LDHL_SP_n(); break;
		case 0xF9:	_inst->LD_SP_HL(); break;
		case 0xFA:	_inst->LD_A_n(n16); break;
		case 0xFB:	_inst->EI(IME); break;
		case 0xFE:	_inst->CP_n(n8); break;
		case 0xFF:	_inst->RST_n(0x38); break;
		default:
			cout << hex << "Unimplemented instruction \"" << (int)OPCODE << "\" at PC = "
				<< _registers.getPC() << dec << endl;
			return -1;
			break;
		}
	}

	if (CBcycles == 0)
	{
		if (_condTaken)
		{
			condCycles();
			return _instCycles[OPCODE];
		}
		else
		{
			uncondCycles();
			return _instCycles[OPCODE];
		}
	}
	else return CBcycles;
}

int CPU::CB_prefix(Instructions *inst)
{
	_registers.addPC(1);
	BYTE OPCODE = _memory->read(_registers.getPC());

	switch (OPCODE)
	{
	case 0x00:	_inst->RLC_n(B); break;
	case 0x01:	_inst->RLC_n(C); break;
	case 0x02:	_inst->RLC_n(D); break;
	case 0x03:	_inst->RLC_n(E); break;
	case 0x04:	_inst->RLC_n(H); break;
	case 0x05:	_inst->RLC_n(L); break;
	case 0x06:	_inst->RLC_n(mHL); break;
	case 0x07:	_inst->RLC_n(A); break;
	case 0x08:	_inst->RRC_n(B); break;
	case 0x09:	_inst->RRC_n(C); break;
	case 0x0A:	_inst->RRC_n(D); break;
	case 0x0B:	_inst->RRC_n(E); break;
	case 0x0C:	_inst->RRC_n(H); break;
	case 0x0D:	_inst->RRC_n(L); break;
	case 0x0E:	_inst->RRC_n(mHL); break;
	case 0x0F:	_inst->RRC_n(A); break;
	case 0x10:	_inst->RL_n(B); break;
	case 0x11:	_inst->RL_n(C); break;
	case 0x12:	_inst->RL_n(D); break;
	case 0x13:	_inst->RL_n(E); break;
	case 0x14:	_inst->RL_n(H); break;
	case 0x15:	_inst->RL_n(L); break;
	case 0x16:	_inst->RL_n(mHL); break;
	case 0x17:	_inst->RL_n(A); break;
	case 0x18:	_inst->RR_n(B); break;
	case 0x19:	_inst->RR_n(C); break;
	case 0x1A:	_inst->RR_n(D); break;
	case 0x1B:	_inst->RR_n(E); break;
	case 0x1C:	_inst->RR_n(H); break;
	case 0x1D:	_inst->RR_n(L); break;
	case 0x1E:	_inst->RR_n(mHL); break;
	case 0x1f:	_inst->RR_n(A); break;
	case 0x20:	_inst->SLA_n(B); break;
	case 0x21:	_inst->SLA_n(C); break;
	case 0x22:	_inst->SLA_n(D); break;
	case 0x23:	_inst->SLA_n(E); break;
	case 0x24:	_inst->SLA_n(H); break;
	case 0x25:	_inst->SLA_n(L); break;
	case 0x26:	_inst->SLA_n(mHL); break;
	case 0x27:	_inst->SLA_n(A); break;
	case 0x28:	_inst->SRA_n(B); break;
	case 0x29:	_inst->SRA_n(C); break;
	case 0x2A:	_inst->SRA_n(D); break;
	case 0x2B:	_inst->SRA_n(E); break;
	case 0x2C:	_inst->SRA_n(H); break;
	case 0x2D:	_inst->SRA_n(L); break;
	case 0x2E:	_inst->SRA_n(mHL); break;
	case 0x2F:	_inst->SRA_n(A); break;
	case 0x30:	_inst->SWAP_n(B); break;
	case 0x31:	_inst->SWAP_n(C); break;
	case 0x32:	_inst->SWAP_n(D); break;
	case 0x33:	_inst->SWAP_n(E); break;
	case 0x34:	_inst->SWAP_n(H); break;
	case 0x35:	_inst->SWAP_n(L); break;
	case 0x36:	_inst->SWAP_n(mHL); break;
	case 0x37:	_inst->SWAP_n(A); break;
	case 0x38:	_inst->SRL_n(B); break;
	case 0x39:	_inst->SRL_n(C); break;
	case 0x3A:	_inst->SRL_n(D); break;
	case 0x3B:	_inst->SRL_n(E); break;
	case 0x3C:	_inst->SRL_n(H); break;
	case 0x3D:	_inst->SRL_n(L); break;
	case 0x3E:	_inst->SRL_n(mHL); break;
	case 0x3F:	_inst->SRL_n(A); break;
	case 0x40:	_inst->BIT_b_r(0, B); break;
	case 0x41:	_inst->BIT_b_r(0, C); break;
	case 0x42:	_inst->BIT_b_r(0, D); break;
	case 0x43:	_inst->BIT_b_r(0, E); break;
	case 0x44:	_inst->BIT_b_r(0, H); break;
	case 0x45:	_inst->BIT_b_r(0, L); break;
	case 0x46:	_inst->BIT_b_r(0, mHL); break;
	case 0x47:	_inst->BIT_b_r(0, A); break;
	case 0x48:	_inst->BIT_b_r(1, B); break;
	case 0x49:	_inst->BIT_b_r(1, C); break;
	case 0x4A:	_inst->BIT_b_r(1, D); break;
	case 0x4B:	_inst->BIT_b_r(1, E); break;
	case 0x4C:	_inst->BIT_b_r(1, H); break;
	case 0x4D:	_inst->BIT_b_r(1, L); break;
	case 0x4E:	_inst->BIT_b_r(1, mHL); break;
	case 0x4F:	_inst->BIT_b_r(1, A); break;
	case 0x50:	_inst->BIT_b_r(2, B); break;
	case 0x51:	_inst->BIT_b_r(2, C); break;
	case 0x52:	_inst->BIT_b_r(2, D); break;
	case 0x53:	_inst->BIT_b_r(2, E); break;
	case 0x54:	_inst->BIT_b_r(2, H); break;
	case 0x55:	_inst->BIT_b_r(2, L); break;
	case 0x56:	_inst->BIT_b_r(2, mHL); break;
	case 0x57:	_inst->BIT_b_r(2, A); break;
	case 0x58:	_inst->BIT_b_r(3, B); break;
	case 0x59:	_inst->BIT_b_r(3, C); break;
	case 0x5A:	_inst->BIT_b_r(3, D); break;
	case 0x5B:	_inst->BIT_b_r(3, E); break;
	case 0x5C:	_inst->BIT_b_r(3, H); break;
	case 0x5D:	_inst->BIT_b_r(3, L); break;
	case 0x5E:	_inst->BIT_b_r(3, mHL); break;
	case 0x5F:	_inst->BIT_b_r(3, A); break;
	case 0x60:	_inst->BIT_b_r(4, B); break;
	case 0x61:	_inst->BIT_b_r(4, C); break;
	case 0x62:	_inst->BIT_b_r(4, D); break;
	case 0x63:	_inst->BIT_b_r(4, E); break;
	case 0x64:	_inst->BIT_b_r(4, H); break;
	case 0x65:	_inst->BIT_b_r(4, L); break;
	case 0x66:	_inst->BIT_b_r(4, mHL); break;
	case 0x67:	_inst->BIT_b_r(4, A); break;
	case 0x68:	_inst->BIT_b_r(5, B); break;
	case 0x69:	_inst->BIT_b_r(5, C); break;
	case 0x6A:	_inst->BIT_b_r(5, D); break;
	case 0x6B:	_inst->BIT_b_r(5, E); break;
	case 0x6C:	_inst->BIT_b_r(5, H); break;
	case 0x6D:	_inst->BIT_b_r(5, L); break;
	case 0x6E:	_inst->BIT_b_r(5, mHL); break;
	case 0x6F:	_inst->BIT_b_r(5, A); break;
	case 0x70:	_inst->BIT_b_r(6, B); break;
	case 0x71:	_inst->BIT_b_r(6, C); break;
	case 0x72:	_inst->BIT_b_r(6, D); break;
	case 0x73:	_inst->BIT_b_r(6, E); break;
	case 0x74:	_inst->BIT_b_r(6, H); break;
	case 0x75:	_inst->BIT_b_r(6, L); break;
	case 0x76:	_inst->BIT_b_r(6, mHL); break;
	case 0x77:	_inst->BIT_b_r(6, A); break;
	case 0x78:	_inst->BIT_b_r(7, B); break;
	case 0x79:	_inst->BIT_b_r(7, C); break;
	case 0x7A:	_inst->BIT_b_r(7, D); break;
	case 0x7B:	_inst->BIT_b_r(7, E); break;
	case 0x7C:	_inst->BIT_b_r(7, H); break;
	case 0x7D:	_inst->BIT_b_r(7, L); break;
	case 0x7E:	_inst->BIT_b_r(7, mHL); break;
	case 0x7F:	_inst->BIT_b_r(7, A); break;
	case 0x80:	_inst->RES_b_r(0, B); break;
	case 0x81:	_inst->RES_b_r(0, C); break;
	case 0x82:	_inst->RES_b_r(0, D); break;
	case 0x83:	_inst->RES_b_r(0, E); break;
	case 0x84:	_inst->RES_b_r(0, H); break;
	case 0x85:	_inst->RES_b_r(0, L); break;
	case 0x86:	_inst->RES_b_r(0, mHL); break;
	case 0x87:	_inst->RES_b_r(0, A); break;
	case 0x88:	_inst->RES_b_r(1, B); break;
	case 0x89:	_inst->RES_b_r(1, C); break;
	case 0x8A:	_inst->RES_b_r(1, D); break;
	case 0x8B:	_inst->RES_b_r(1, E); break;
	case 0x8C:	_inst->RES_b_r(1, H); break;
	case 0x8D:	_inst->RES_b_r(1, L); break;
	case 0x8E:	_inst->RES_b_r(1, mHL); break;
	case 0x8F:	_inst->RES_b_r(1, A); break;
	case 0x90:	_inst->RES_b_r(2, B); break;
	case 0x91:	_inst->RES_b_r(2, C); break;
	case 0x92:	_inst->RES_b_r(2, D); break;
	case 0x93:	_inst->RES_b_r(2, E); break;
	case 0x94:	_inst->RES_b_r(2, H); break;
	case 0x95:	_inst->RES_b_r(2, L); break;
	case 0x96:	_inst->RES_b_r(2, mHL); break;
	case 0x97:	_inst->RES_b_r(2, A); break;
	case 0x98:	_inst->RES_b_r(3, B); break;
	case 0x99:	_inst->RES_b_r(3, C); break;
	case 0x9A:	_inst->RES_b_r(3, D); break;
	case 0x9B:	_inst->RES_b_r(3, E); break;
	case 0x9C:	_inst->RES_b_r(3, H); break;
	case 0x9D:	_inst->RES_b_r(3, L); break;
	case 0x9E:	_inst->RES_b_r(3, mHL); break;
	case 0x9F:	_inst->RES_b_r(3, A); break;
	case 0xA0:	_inst->RES_b_r(4, B); break;
	case 0xA1:	_inst->RES_b_r(4, C); break;
	case 0xA2:	_inst->RES_b_r(4, D); break;
	case 0xA3:	_inst->RES_b_r(4, E); break;
	case 0xA4:	_inst->RES_b_r(4, H); break;
	case 0xA5:	_inst->RES_b_r(4, L); break;
	case 0xA6:	_inst->RES_b_r(4, mHL); break;
	case 0xA7:	_inst->RES_b_r(4, A); break;
	case 0xA8:	_inst->RES_b_r(5, B); break;
	case 0xA9:	_inst->RES_b_r(5, C); break;
	case 0xAA:	_inst->RES_b_r(5, D); break;
	case 0xAB:	_inst->RES_b_r(5, E); break;
	case 0xAC:	_inst->RES_b_r(5, H); break;
	case 0xAD:	_inst->RES_b_r(5, L); break;
	case 0xAE:	_inst->RES_b_r(5, mHL); break;
	case 0xAF:	_inst->RES_b_r(5, A); break;
	case 0xB0:	_inst->RES_b_r(6, B); break;
	case 0xB1:	_inst->RES_b_r(6, C); break;
	case 0xB2:	_inst->RES_b_r(6, D); break;
	case 0xB3:	_inst->RES_b_r(6, E); break;
	case 0xB4:	_inst->RES_b_r(6, H); break;
	case 0xB5:	_inst->RES_b_r(6, L); break;
	case 0xB6:	_inst->RES_b_r(6, mHL); break;
	case 0xB7:	_inst->RES_b_r(6, A); break;
	case 0xB8:	_inst->RES_b_r(7, B); break;
	case 0xB9:	_inst->RES_b_r(7, C); break;
	case 0xBA:	_inst->RES_b_r(7, D); break;
	case 0xBB:	_inst->RES_b_r(7, E); break;
	case 0xBC:	_inst->RES_b_r(7, H); break;
	case 0xBD:	_inst->RES_b_r(7, L); break;
	case 0xBE:	_inst->RES_b_r(7, mHL); break;
	case 0xBF:	_inst->RES_b_r(7, A); break;
	case 0xC0:	_inst->SET_b_r(0, B); break;
	case 0xC1:	_inst->SET_b_r(0, C); break;
	case 0xC2:	_inst->SET_b_r(0, D); break;
	case 0xC3:	_inst->SET_b_r(0, E); break;
	case 0xC4:	_inst->SET_b_r(0, H); break;
	case 0xC5:	_inst->SET_b_r(0, L); break;
	case 0xC6:	_inst->SET_b_r(0, mHL); break;
	case 0xC7:	_inst->SET_b_r(0, A); break;
	case 0xC8:	_inst->SET_b_r(1, B); break;
	case 0xC9:	_inst->SET_b_r(1, C); break;
	case 0xCA:	_inst->SET_b_r(1, D); break;
	case 0xCB:	_inst->SET_b_r(1, E); break;
	case 0xCC:	_inst->SET_b_r(1, H); break;
	case 0xCD:	_inst->SET_b_r(1, L); break;
	case 0xCE:	_inst->SET_b_r(1, mHL); break;
	case 0xCF:	_inst->SET_b_r(1, A); break;
	case 0xD0:	_inst->SET_b_r(2, B); break;
	case 0xD1:	_inst->SET_b_r(2, C); break;
	case 0xD2:	_inst->SET_b_r(2, D); break;
	case 0xD3:	_inst->SET_b_r(2, E); break;
	case 0xD4:	_inst->SET_b_r(2, H); break;
	case 0xD5:	_inst->SET_b_r(2, L); break;
	case 0xD6:	_inst->SET_b_r(2, mHL); break;
	case 0xD7:	_inst->SET_b_r(2, A); break;
	case 0xD8:	_inst->SET_b_r(3, B); break;
	case 0xD9:	_inst->SET_b_r(3, C); break;
	case 0xDA:	_inst->SET_b_r(3, D); break;
	case 0xDB:	_inst->SET_b_r(3, E); break;
	case 0xDC:	_inst->SET_b_r(3, H); break;
	case 0xDD:	_inst->SET_b_r(3, L); break;
	case 0xDE:	_inst->SET_b_r(3, mHL); break;
	case 0xDF:	_inst->SET_b_r(3, A); break;
	case 0xE0:	_inst->SET_b_r(4, B); break;
	case 0xE1:	_inst->SET_b_r(4, C); break;
	case 0xE2:	_inst->SET_b_r(4, D); break;
	case 0xE3:	_inst->SET_b_r(4, E); break;
	case 0xE4:	_inst->SET_b_r(4, H); break;
	case 0xE5:	_inst->SET_b_r(4, L); break;
	case 0xE6:	_inst->SET_b_r(4, mHL); break;
	case 0xE7:	_inst->SET_b_r(4, A); break;
	case 0xE8:	_inst->SET_b_r(5, B); break;
	case 0xE9:	_inst->SET_b_r(5, C); break;
	case 0xEA:	_inst->SET_b_r(5, D); break;
	case 0xEB:	_inst->SET_b_r(5, E); break;
	case 0xEC:	_inst->SET_b_r(5, H); break;
	case 0xED:	_inst->SET_b_r(5, L); break;
	case 0xEE:	_inst->SET_b_r(5, mHL); break;
	case 0xEF:	_inst->SET_b_r(5, A); break;
	case 0xF0:	_inst->SET_b_r(6, B); break;
	case 0xF1:	_inst->SET_b_r(6, C); break;
	case 0xF2:	_inst->SET_b_r(6, D); break;
	case 0xF3:	_inst->SET_b_r(6, E); break;
	case 0xF4:	_inst->SET_b_r(6, H); break;
	case 0xF5:	_inst->SET_b_r(6, L); break;
	case 0xF6:	_inst->SET_b_r(6, mHL); break;
	case 0xF7:	_inst->SET_b_r(6, A); break;
	case 0xF8:	_inst->SET_b_r(7, B); break;
	case 0xF9:	_inst->SET_b_r(7, C); break;
	case 0xFA:	_inst->SET_b_r(7, D); break;
	case 0xFB:	_inst->SET_b_r(7, E); break;
	case 0xFC:	_inst->SET_b_r(7, H); break;
	case 0xFD:	_inst->SET_b_r(7, L); break;
	case 0xFE:	_inst->SET_b_r(7, mHL); break;
	case 0xFF:	_inst->SET_b_r(7, A); break;

	default:
		cout << hex << "Unimplemented CB instruction \"" << (int)OPCODE << "\" at PC = "
			<< _registers.getPC() << dec << endl;
		return -1;
		break;
	}
	return _CBinstCycles[OPCODE];
}

void CPU::initCyclesArrays()
{
	for (int i = 0x00; i <= 0xFF; i++)
	{
		_instCycles[i] = 4;
		_CBinstCycles[i] = 8;
	}

	// Instructions cycles
	_instCycles[0x01] = 12;
	_instCycles[0x02] = 8;
	_instCycles[0x03] = 8;
	_instCycles[0x06] = 8;
	_instCycles[0x09] = 8;
	_instCycles[0x0A] = 8;
	_instCycles[0x0B] = 8;
	_instCycles[0x0E] = 8;
	_instCycles[0x11] = 12;
	_instCycles[0x12] = 8;
	_instCycles[0x13] = 8;
	_instCycles[0x16] = 8;
	_instCycles[0x18] = 12;
	_instCycles[0x19] = 8;
	_instCycles[0x1A] = 8;
	_instCycles[0x1B] = 8;
	_instCycles[0x1E] = 8;
	_instCycles[0x20] = 8;
	_instCycles[0x21] = 12;
	_instCycles[0x22] = 8;
	_instCycles[0x23] = 8;
	_instCycles[0x26] = 8;
	_instCycles[0x28] = 8;
	_instCycles[0x29] = 8;
	_instCycles[0x2A] = 8;
	_instCycles[0x2B] = 8;
	_instCycles[0x2E] = 8;
	_instCycles[0x30] = 8;
	_instCycles[0x31] = 12;
	_instCycles[0x32] = 8;
	_instCycles[0x33] = 8;
	_instCycles[0x34] = 12;
	_instCycles[0x35] = 12;
	_instCycles[0x36] = 12;
	_instCycles[0x38] = 8;
	_instCycles[0x39] = 8;
	_instCycles[0x3A] = 8;
	_instCycles[0x3B] = 8;
	_instCycles[0x3E] = 8;
	_instCycles[0x46] = 8;
	_instCycles[0x4E] = 8;
	_instCycles[0x56] = 8;
	_instCycles[0x5E] = 8;
	_instCycles[0x66] = 8;
	_instCycles[0x6E] = 8;
	_instCycles[0x7E] = 8;
	_instCycles[0x86] = 8;
	_instCycles[0x8E] = 8;
	_instCycles[0x96] = 8;
	_instCycles[0x9E] = 8;
	_instCycles[0xA6] = 8;
	_instCycles[0xAE] = 8;
	_instCycles[0xB6] = 8;
	_instCycles[0xBE] = 8;
	_instCycles[0xC0] = 8;
	_instCycles[0xC1] = 12;
	_instCycles[0xC2] = 12;
	_instCycles[0xC3] = 16;
	_instCycles[0xC4] = 12;
	_instCycles[0xC5] = 16;
	_instCycles[0xC6] = 8;
	_instCycles[0xC8] = 8;
	_instCycles[0xCE] = 8;
	_instCycles[0xCF] = 16;
	_instCycles[0xD0] = 8;
	_instCycles[0xD1] = 12;
	_instCycles[0xD2] = 12;
	_instCycles[0xD4] = 12;
	_instCycles[0xD5] = 16;
	_instCycles[0xD6] = 8;
	_instCycles[0xD7] = 16;
	_instCycles[0xD8] = 8;
	_instCycles[0xD9] = 16;
	_instCycles[0xDA] = 12;
	_instCycles[0xDC] = 12;
	_instCycles[0xDE] = 8;
	_instCycles[0xDF] = 16;
	_instCycles[0xE6] = 8;
	_instCycles[0xE0] = 12;
	_instCycles[0xE1] = 12;
	_instCycles[0xE2] = 8;
	_instCycles[0xE5] = 16;
	_instCycles[0xE6] = 8;
	_instCycles[0xE7] = 16;
	_instCycles[0xE8] = 16;
	_instCycles[0xEA] = 16;
	_instCycles[0xEE] = 8;
	_instCycles[0xEF] = 16;
	_instCycles[0xF0] = 12;
	_instCycles[0xF1] = 12;
	_instCycles[0xF2] = 8;
	_instCycles[0xF5] = 16;
	_instCycles[0xF6] = 8;
	_instCycles[0xF7] = 16;
	_instCycles[0xF8] = 12;
	_instCycles[0xF9] = 8;
	_instCycles[0xFA] = 16;
	_instCycles[0xFE] = 8;
	_instCycles[0xFF] = 16;

	// CB prefix instructions cycles
	_CBinstCycles[0x06] = 16;
	_CBinstCycles[0x0E] = 16;
	_CBinstCycles[0x16] = 16;
	_CBinstCycles[0x1E] = 16;
	_CBinstCycles[0x26] = 16;
	_CBinstCycles[0x2E] = 16;
	_CBinstCycles[0x36] = 16;
	_CBinstCycles[0x3E] = 16;
	_CBinstCycles[0x46] = 16;
	_CBinstCycles[0x4E] = 16;
	_CBinstCycles[0x56] = 16;
	_CBinstCycles[0x5E] = 16;
	_CBinstCycles[0x66] = 16;
	_CBinstCycles[0x6E] = 16;
	_CBinstCycles[0x76] = 16;
	_CBinstCycles[0x7E] = 16;
	_CBinstCycles[0x86] = 16;
	_CBinstCycles[0x8E] = 16;
	_CBinstCycles[0x96] = 16;
	_CBinstCycles[0x9E] = 16;
	_CBinstCycles[0xA6] = 16;
	_CBinstCycles[0xAE] = 16;
	_CBinstCycles[0xB6] = 16;
	_CBinstCycles[0xBE] = 16;
	_CBinstCycles[0xC6] = 16;
	_CBinstCycles[0xCE] = 16;
	_CBinstCycles[0xD6] = 16;
	_CBinstCycles[0xDE] = 16;
	_CBinstCycles[0xE6] = 16;
	_CBinstCycles[0xEE] = 16;
	_CBinstCycles[0xF6] = 16;
	_CBinstCycles[0xFE] = 16;
}

void CPU::condCycles()
{
	_instCycles[0x20] = 12;
	_instCycles[0x28] = 12;
	_instCycles[0x30] = 12;
	_instCycles[0x38] = 12;
	_instCycles[0xC0] = 20;
	_instCycles[0xC2] = 16;
	_instCycles[0xC4] = 24;
	_instCycles[0xC8] = 20;
	_instCycles[0xCA] = 16;
	_instCycles[0xCC] = 24;
	_instCycles[0xD0] = 20;
	_instCycles[0xD2] = 16;
	_instCycles[0xD4] = 24;
	_instCycles[0xD8] = 20;
	_instCycles[0xDA] = 16;
	_instCycles[0xDC] = 24;
}

void CPU::uncondCycles()
{
	_instCycles[0x20] = 8;
	_instCycles[0x28] = 8;
	_instCycles[0x30] = 8;
	_instCycles[0x38] = 8;
	_instCycles[0xC0] = 8;
	_instCycles[0xC2] = 12;
	_instCycles[0xC4] = 12;
	_instCycles[0xC8] = 8;
	_instCycles[0xCA] = 12;
	_instCycles[0xCC] = 12;
	_instCycles[0xD0] = 8;
	_instCycles[0xD2] = 12;
	_instCycles[0xD4] = 12;
	_instCycles[0xD8] = 8;
	_instCycles[0xDA] = 12;
	_instCycles[0xDC] = 12;
}

void CPU::requestInterrupt(int id)
{
	/*switch (id)
	{
	case VBlank: cout << "VBLANK requested" << endl; break;
	case LCD: cout << "LCD requested" << endl; break;
	case Timer: cout << "Timer requested" << endl; break;
	case JoyPad: cout << "JoyPad requested" << endl; break;
	default:
		break;
	}*/

	BYTE reqInt = _memory->read(0xFF0F);
	//cout << "REQ_BEF: " << hex << (int)reqInt << endl;
	reqInt = bitSet(reqInt, id);
	_memory->write(0xFF0F, reqInt);

	if (testBit(_memory->read(0xFFFF), id))
		_registers.setHalt(false);
	//cout << "REQ_AFT: " << hex << (int)reqInt << endl;
	//cout << "ENAB: " << hex << (int)_memory->read(0xFFFF) << endl;
}

void CPU::doInterrupts()
{
	if (IME)
	{
		BYTE reqInt = _memory->read(0xFF0F);
		BYTE enabInt = _memory->read(0xFFFF);

		//cout << "REQ: " << hex << (int)reqInt << endl;
		//cout << "ENAB: " << hex << (int)enabInt << endl;

		if (reqInt > 0x00)
		{
			for (int i = 0; i < 5; i++)
			{
				if (testBit(reqInt, i))
				{
					if (testBit(enabInt, i))
						serviceInterrupt(i);
				}
			}
		}
	}
}

void CPU::serviceInterrupt(int id)
{
	//cout << "Interrupt Serviced" << endl;
	IME = false;
	BYTE reqInt = _memory->read(0xFF0F);
	reqInt = bitReset(reqInt, id);
	_memory->write(0xFF0F, reqInt);

	_registers.setSP(_registers.getSP() - 1);
	_memory->write(_registers.getSP(), (_registers.getPC() & 0xFF00) >> 8);
	_registers.setSP(_registers.getSP() - 1);
	_memory->write(_registers.getSP(), _registers.getPC() & 0x00FF);

	
	/*switch (id)
	{
	case VBlank: cout << "VBLANK serviced" << endl; break;
	case LCD: cout << "LCD serviced" << endl; break;
	case Timer: cout << "Timer serviced" << endl; break;
	case JoyPad: cout << "JoyPad Serviced" << endl; break;
	default:
		break;
	}*/

	_registers.setHalt(false);

	switch (id)
	{
	case VBlank: _registers.setPC(0x40); break;
	case LCD: _registers.setPC(0x48); break;
	case Timer: _registers.setPC(0x50); break;
	case CLink: break; // Cable Link
	case JoyPad: _registers.setPC(0x60); break;
	}
}


void CPU::updateTimers(int cycles)
{
	//cout << hex << (int)_memory->read(TAC) << endl;
	divideRegister(cycles);
	if (isClockEnabled())
	{
		_timeCounter -= cycles;

		if (_timeCounter <= 0)
		{
			setClockFreq();

			if (_memory->read(TIMA) == 255)
			{
				_memory->write(TIMA, _memory->read(TMA));
				requestInterrupt(Timer);
			}
			else _memory->write(TIMA, _memory->read(TIMA) + 1);
		}
	}
}

BYTE CPU::getClockFreq() const
{
	return _memory->read(TAC) & 0x3;
}

void CPU::setClockFreq()
{
	BYTE freq = getClockFreq();
	switch (freq)
	{
	case 0: _timeCounter = 1024; break; // freq 4096
	case 1: _timeCounter = 16; break; // freq 262144
	case 2: _timeCounter = 64; break; // freq 65536
	case 3: _timeCounter = 256; break; // freq 16382
	}
}

bool CPU::isClockEnabled()
{
	//cout << hex << (int)_memory->read(TAC) << endl;
	return testBit(_memory->read(TAC), 2) ? true : false;
}

void CPU::divideRegister(int cycles)
{
	_divideCounter += cycles;
	if (_divideCounter >= 255)
	{
		_divideCounter = 0;
		_memory->directModification(0xFF04, _memory->read(0xFF04) + 1);
	}
}