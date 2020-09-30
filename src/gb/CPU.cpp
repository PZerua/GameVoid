#include "CPU.h"
 
#include "bitUtils.h"

void CPU::init(Memory *memory)
{
    m_memory = memory;
    m_inst = new Instructions(m_memory, &m_registers);
    m_timeCounter = 0;
    m_divideCounter = 0;
    initCyclesArrays();
    m_IME = true;
}

int CPU::fetch()
{
    bool condTaken = false;
    int CBcycles = 0;
    static bool debug = false;
    static int count = 0;

    BYTE OPCODE = m_memory->read(m_registers.getPC());

    if (debug) 
    {
        lastPC = m_registers.getPC();
        lastOPCODE = OPCODE;

        cout << hex << "PC: " << (int)m_registers.getPC() << " | OPCODE: " << (int)OPCODE << "\t";
        cout << hex << "AF: " << (int)m_registers.getAF() << " | BC: " << (int)m_registers.getBC() << " | DE: " << (int)m_registers.getDE()
            << " | HL: " << (int)m_registers.getHL() << " | SP: " << (int)m_registers.getSP() << endl;
        count++;
    }
    
    if (!m_registers.haltEnabled())
    {
        switch (OPCODE)
        {
        case 0x00:    m_inst->NOP(); break;
        case 0x01:    m_inst->LD_n_nn(bc); break;
        case 0x02:    m_inst->LD_n_A(mBC); break;
        case 0x03:    m_inst->INC_nn(bc); break;
        case 0x04:    m_inst->INC_n(B); break;
        case 0x05:    m_inst->DEC_n(B); break;
        case 0x06:    m_inst->LD_nn_n(B); break;
        case 0x07:    m_inst->RLCA(); break;
        case 0x08:    m_inst->LD_nn_SP(); break;
        case 0x09:    m_inst->ADD_HL_n(bc); break;
        case 0x0A:    m_inst->LD_A_n(mBC); break;
        case 0x0B:    m_inst->DEC_nn(bc); break;
        case 0x0C:    m_inst->INC_n(C); break;
        case 0x0D:    m_inst->DEC_n(C); break;
        case 0x0E:    m_inst->LD_nn_n(C); break;
        case 0x0F:    m_inst->RRCA(); break;
        case 0x10:    m_inst->STOP(); break;
        case 0x11:    m_inst->LD_n_nn(de); break;
        case 0x12:    m_inst->LD_n_A(mDE); break;
        case 0x13:    m_inst->INC_nn(de); break;
        case 0x14:    m_inst->INC_n(D); break;
        case 0x15:    m_inst->DEC_n(D); break;
        case 0x16:    m_inst->LD_nn_n(D); break;
        case 0x17:    m_inst->RLA(); break;
        case 0x18:    m_inst->JR_n(); break;
        case 0x19:    m_inst->ADD_HL_n(de); break;
        case 0x1A:    m_inst->LD_A_n(mDE); break;
        case 0x1B:    m_inst->DEC_nn(de); break;
        case 0x1C:    m_inst->INC_n(E); break;
        case 0x1D:    m_inst->DEC_n(E); break;
        case 0x1E:    m_inst->LD_nn_n(E); break;
        case 0x1F:    m_inst->RRA(); break;
        case 0x20:    m_inst->JR_cc_n(nZ, condTaken); break;
        case 0x21:    m_inst->LD_n_nn(hl); break;
        case 0x22:    m_inst->LD_HLI_A(); break;
        case 0x23:    m_inst->INC_nn(hl); break;
        case 0x24:    m_inst->INC_n(H); break;
        case 0x25:    m_inst->DEC_n(H); break;
        case 0x26:    m_inst->LD_nn_n(H); break;
        case 0x27:    m_inst->DAA(); break;
        case 0x28:    m_inst->JR_cc_n(Z, condTaken); break;
        case 0x29:    m_inst->ADD_HL_n(hl); break;
        case 0x2A:    m_inst->LD_A_HLI(); break;
        case 0x2B:    m_inst->DEC_nn(hl); break;
        case 0x2C:    m_inst->INC_n(L); break;
        case 0x2D:    m_inst->DEC_n(L); break;
        case 0x2E:    m_inst->LD_nn_n(L); break;
        case 0x2F:    m_inst->CPL(); break;
        case 0x30:    m_inst->JR_cc_n(nC, condTaken); break;
        case 0x31:    m_inst->LD_n_nn(sp); break;
        case 0x32:    m_inst->LD_HLD_A(); break;
        case 0x33:    m_inst->INC_nn(sp); break;
        case 0x34:    m_inst->INC_n(mHL); break;
        case 0x35:    m_inst->DEC_n(mHL); break;
        case 0x36:    m_inst->LD_r1_r2(mHL, n8); break;
        case 0x37:    m_inst->SCF(); break;
        case 0x38:    m_inst->JR_cc_n(sC, condTaken); break;
        case 0x39:    m_inst->ADD_HL_n(sp); break;
        case 0x3A:    m_inst->LD_A_HLD(); break;
        case 0x3B:    m_inst->DEC_nn(sp); break;
        case 0x3C:    m_inst->INC_n(A); break;
        case 0x3D:    m_inst->DEC_n(A); break;
        case 0x3E:    m_inst->LD_A_n(n8); break;
        case 0x3F:    m_inst->CCF(); break;
        case 0x40:    m_inst->LD_r1_r2(B, B); break;
        case 0x41:    m_inst->LD_r1_r2(B, C); break;
        case 0x42:    m_inst->LD_r1_r2(B, D); break;
        case 0x43:    m_inst->LD_r1_r2(B, E); break;
        case 0x44:    m_inst->LD_r1_r2(B, H); break;
        case 0x45:    m_inst->LD_r1_r2(B, L); break;
        case 0x46:    m_inst->LD_r1_r2(B, mHL); break;
        case 0x47:    m_inst->LD_n_A(B); break;
        case 0x48:    m_inst->LD_r1_r2(C, B); break;
        case 0x49:    m_inst->LD_r1_r2(C, C); break;
        case 0x4A:    m_inst->LD_r1_r2(C, D); break;
        case 0x4B:    m_inst->LD_r1_r2(C, E); break;
        case 0x4C:    m_inst->LD_r1_r2(C, H); break;
        case 0x4D:    m_inst->LD_r1_r2(C, L); break;
        case 0x4E:    m_inst->LD_r1_r2(C, mHL); break;
        case 0x4F:    m_inst->LD_n_A(C); break;
        case 0x50:    m_inst->LD_r1_r2(D, B); break;
        case 0x51:    m_inst->LD_r1_r2(D, C); break;
        case 0x52:    m_inst->LD_r1_r2(D, D); break;
        case 0x53:    m_inst->LD_r1_r2(D, E); break;
        case 0x54:    m_inst->LD_r1_r2(D, H); break;
        case 0x55:    m_inst->LD_r1_r2(D, L); break;
        case 0x56:    m_inst->LD_r1_r2(D, mHL); break;
        case 0x57:    m_inst->LD_n_A(D); break;
        case 0x58:    m_inst->LD_r1_r2(E, B); break;
        case 0x59:    m_inst->LD_r1_r2(E, C); break;
        case 0x5A:    m_inst->LD_r1_r2(E, D); break;
        case 0x5B:    m_inst->LD_r1_r2(E, E); break;
        case 0x5C:    m_inst->LD_r1_r2(E, H); break;
        case 0x5D:    m_inst->LD_r1_r2(E, L); break;
        case 0x5E:    m_inst->LD_r1_r2(E, mHL); break;
        case 0x5F:    m_inst->LD_n_A(E); break;
        case 0x60:    m_inst->LD_r1_r2(H, B); break;
        case 0x61:    m_inst->LD_r1_r2(H, C); break;
        case 0x62:    m_inst->LD_r1_r2(H, D); break;
        case 0x63:    m_inst->LD_r1_r2(H, E); break;
        case 0x64:    m_inst->LD_r1_r2(H, H); break;
        case 0x65:    m_inst->LD_r1_r2(H, L); break;
        case 0x66:    m_inst->LD_r1_r2(H, mHL); break;
        case 0x67:    m_inst->LD_n_A(H); break;
        case 0x68:    m_inst->LD_r1_r2(L, B); break;
        case 0x69:    m_inst->LD_r1_r2(L, C); break;
        case 0x6A:    m_inst->LD_r1_r2(L, D); break;
        case 0x6B:    m_inst->LD_r1_r2(L, E); break;
        case 0x6C:    m_inst->LD_r1_r2(L, H); break;
        case 0x6D:    m_inst->LD_r1_r2(L, L); break;
        case 0x6E:    m_inst->LD_r1_r2(L, mHL); break;
        case 0x6F:    m_inst->LD_n_A(L); break;
        case 0x70:    m_inst->LD_r1_r2(mHL, B); break;
        case 0x71:    m_inst->LD_r1_r2(mHL, C); break;
        case 0x72:    m_inst->LD_r1_r2(mHL, D); break;
        case 0x73:    m_inst->LD_r1_r2(mHL, E); break;
        case 0x74:    m_inst->LD_r1_r2(mHL, H); break;
        case 0x75:    m_inst->LD_r1_r2(mHL, L); break;
        case 0x76:    m_inst->HALT(); break;
        case 0x77:    m_inst->LD_n_A(mHL); break;
        case 0x78:    m_inst->LD_A_n(B); break;
        case 0x79:    m_inst->LD_A_n(C); break;
        case 0x7A:    m_inst->LD_A_n(D); break;
        case 0x7B:    m_inst->LD_A_n(E); break;
        case 0x7C:    m_inst->LD_A_n(H); break;
        case 0x7D:    m_inst->LD_A_n(L); break;
        case 0x7E:    m_inst->LD_A_n(mHL); break;
        case 0x7F:    m_inst->LD_A_n(A); break;
        case 0x80:    m_inst->ADD_A_n(B); break;
        case 0x81:    m_inst->ADD_A_n(C); break;
        case 0x82:    m_inst->ADD_A_n(D); break;
        case 0x83:    m_inst->ADD_A_n(E); break;
        case 0x84:    m_inst->ADD_A_n(H); break;
        case 0x85:    m_inst->ADD_A_n(L); break;
        case 0x86:    m_inst->ADD_A_n(mHL); break;
        case 0x87:    m_inst->ADD_A_n(A); break;
        case 0x88:    m_inst->ADC_A_n(B); break;
        case 0x89:    m_inst->ADC_A_n(C); break;
        case 0x8A:    m_inst->ADC_A_n(D); break;
        case 0x8B:    m_inst->ADC_A_n(E); break;
        case 0x8C:    m_inst->ADC_A_n(H); break;
        case 0x8D:    m_inst->ADC_A_n(L); break;
        case 0x8E:    m_inst->ADC_A_n(mHL); break;
        case 0x8F:    m_inst->ADC_A_n(A); break;
        case 0x90:    m_inst->SUB_A_n(B); break;
        case 0x91:    m_inst->SUB_A_n(C); break;
        case 0x92:    m_inst->SUB_A_n(D); break;
        case 0x93:    m_inst->SUB_A_n(E); break;
        case 0x94:    m_inst->SUB_A_n(H); break;
        case 0x95:    m_inst->SUB_A_n(L); break;
        case 0x96:    m_inst->SUB_A_n(mHL); break;
        case 0x97:    m_inst->SUB_A_n(A); break;
        case 0x98:    m_inst->SBC_A_n(B); break;
        case 0x99:    m_inst->SBC_A_n(C); break;
        case 0x9A:    m_inst->SBC_A_n(D); break;
        case 0x9B:    m_inst->SBC_A_n(E); break;
        case 0x9C:    m_inst->SBC_A_n(H); break;
        case 0x9D:    m_inst->SBC_A_n(L); break;
        case 0x9E:    m_inst->SBC_A_n(mHL); break;
        case 0x9F:    m_inst->SBC_A_n(A); break;
        case 0xA0:    m_inst->AND_n(B); break;
        case 0xA1:    m_inst->AND_n(C); break;
        case 0xA2:    m_inst->AND_n(D); break;
        case 0xA3:    m_inst->AND_n(E); break;
        case 0xA4:    m_inst->AND_n(H); break;
        case 0xA5:    m_inst->AND_n(L); break;
        case 0xA6:    m_inst->AND_n(mHL); break;
        case 0xA7:    m_inst->AND_n(A); break;
        case 0xA8:    m_inst->XOR_n(B); break;
        case 0xA9:    m_inst->XOR_n(C); break;
        case 0xAA:    m_inst->XOR_n(D); break;
        case 0xAB:    m_inst->XOR_n(E); break;
        case 0xAC:    m_inst->XOR_n(H); break;
        case 0xAD:    m_inst->XOR_n(L); break;
        case 0xAE:    m_inst->XOR_n(mHL); break;
        case 0xAF:    m_inst->XOR_n(A); break;
        case 0xB0:    m_inst->OR_n(B); break;
        case 0xB1:    m_inst->OR_n(C); break;
        case 0xB2:    m_inst->OR_n(D); break;
        case 0xB3:    m_inst->OR_n(E); break;
        case 0xB4:    m_inst->OR_n(H); break;
        case 0xB5:    m_inst->OR_n(L); break;
        case 0xB6:    m_inst->OR_n(mHL); break;
        case 0xB7:    m_inst->OR_n(A); break;
        case 0xB8:    m_inst->CP_n(B); break;
        case 0xB9:    m_inst->CP_n(C); break;
        case 0xBA:    m_inst->CP_n(D); break;
        case 0xBB:    m_inst->CP_n(E); break;
        case 0xBC:    m_inst->CP_n(H); break;
        case 0xBD:    m_inst->CP_n(L); break;
        case 0xBE:    m_inst->CP_n(mHL); break;
        case 0xBF:    m_inst->CP_n(A); break;
        case 0xC0:    m_inst->RET_cc(nZ, condTaken); break;
        case 0xC1:    m_inst->POP_nn(bc); break;
        case 0xC2:    m_inst->JP_cc_nn(nZ, condTaken); break;
        case 0xC3:    m_inst->JP_nn(); break;
        case 0xC4:    m_inst->CALL_cc_nn(nZ, condTaken); break;
        case 0xC5:    m_inst->PUSH_nn(bc); break;
        case 0xC6:    m_inst->ADD_A_n(n8); break;
        case 0xC7:    m_inst->RST_n(0x00); break;
        case 0xC8:    m_inst->RET_cc(Z, condTaken); break;
        case 0xC9:    m_inst->RET(); break;
        case 0xCA:    m_inst->JP_cc_nn(Z, condTaken); break;
        case 0xCB:    CBcycles = CB_prefix(m_inst); break;
        case 0xCC:    m_inst->CALL_cc_nn(Z, condTaken); break;
        case 0xCD:    m_inst->CALL_nn(); break;
        case 0xCE:    m_inst->ADC_A_n(n8); break;
        case 0xCF:    m_inst->RST_n(0x08); break;
        case 0xD0:    m_inst->RET_cc(nC, condTaken); break;
        case 0xD1:    m_inst->POP_nn(de); break;
        case 0xD2:    m_inst->JP_cc_nn(nC, condTaken); break;
        case 0xD4:    m_inst->CALL_cc_nn(nC, condTaken); break;
        case 0xD5:    m_inst->PUSH_nn(de); break;
        case 0xD6:    m_inst->SUB_A_n(n8); break;
        case 0xD7:    m_inst->RST_n(0x10); break;
        case 0xD8:    m_inst->RET_cc(sC, condTaken); break;
        case 0xD9:    m_inst->RETI(m_IME); break;
        case 0xDA:    m_inst->JP_cc_nn(sC, condTaken); break;
        case 0xDC:    m_inst->CALL_cc_nn(sC, condTaken); break;
        case 0xDE:    m_inst->SBC_A_n(n8); break;
        case 0xDF:    m_inst->RST_n(0x18); break;
        case 0xE0:    m_inst->LDH_n_A(); break;
        case 0xE1:    m_inst->POP_nn(hl); break;
        case 0xE2:    m_inst->LD_C_A(); break;
        case 0xE5:    m_inst->PUSH_nn(hl); break;
        case 0xE6:    m_inst->AND_n(n8); break;
        case 0xE7:    m_inst->RST_n(0x20); break;
        case 0xE8:    m_inst->ADD_SP_n(); break;
        case 0xE9:    m_inst->JP_HL(); break;
        case 0xEA:    m_inst->LD_n_A(n16); break;
        case 0xEE:    m_inst->XOR_n(n8); break;
        case 0xEF:    m_inst->RST_n(0x28); break;
        case 0xF0:    m_inst->LDH_A_n(); break;
        case 0xF1:    m_inst->POP_nn(af); break;
        case 0xF2:    m_inst->LD_A_C(); break;
        case 0xF3:    m_inst->DI(m_IME); break;
        case 0xF5:    m_inst->PUSH_nn(af); break;
        case 0xF6:    m_inst->OR_n(n8); break;
        case 0xF7:    m_inst->RST_n(0x30); break;
        case 0xF8:    m_inst->LDHL_SP_n(); break;
        case 0xF9:    m_inst->LD_SP_HL(); break;
        case 0xFA:    m_inst->LD_A_n(n16); break;
        case 0xFB:    m_inst->EI(m_IME); break;
        case 0xFE:    m_inst->CP_n(n8); break;
        case 0xFF:    m_inst->RST_n(0x38); break;
        default:
            cout << hex << "Unimplemented instruction \"" << (int)OPCODE << "\" at PC = "
                << m_registers.getPC() << dec << endl;
            return -1;
            break;
        }
    }

    if (CBcycles != 0) {
        return CBcycles;
    }
    
    if (condTaken) {
        condCycles();
        return m_instCycles[OPCODE];
    } else {
        uncondCycles();
        return m_instCycles[OPCODE];
    }
}

int CPU::CB_prefix(Instructions *inst)
{
    m_registers.addPC(1);
    BYTE OPCODE = m_memory->read(m_registers.getPC());

    switch (OPCODE) {
    case 0x00:    m_inst->RLC_n(B); break;
    case 0x01:    m_inst->RLC_n(C); break;
    case 0x02:    m_inst->RLC_n(D); break;
    case 0x03:    m_inst->RLC_n(E); break;
    case 0x04:    m_inst->RLC_n(H); break;
    case 0x05:    m_inst->RLC_n(L); break;
    case 0x06:    m_inst->RLC_n(mHL); break;
    case 0x07:    m_inst->RLC_n(A); break;
    case 0x08:    m_inst->RRC_n(B); break;
    case 0x09:    m_inst->RRC_n(C); break;
    case 0x0A:    m_inst->RRC_n(D); break;
    case 0x0B:    m_inst->RRC_n(E); break;
    case 0x0C:    m_inst->RRC_n(H); break;
    case 0x0D:    m_inst->RRC_n(L); break;
    case 0x0E:    m_inst->RRC_n(mHL); break;
    case 0x0F:    m_inst->RRC_n(A); break;
    case 0x10:    m_inst->RL_n(B); break;
    case 0x11:    m_inst->RL_n(C); break;
    case 0x12:    m_inst->RL_n(D); break;
    case 0x13:    m_inst->RL_n(E); break;
    case 0x14:    m_inst->RL_n(H); break;
    case 0x15:    m_inst->RL_n(L); break;
    case 0x16:    m_inst->RL_n(mHL); break;
    case 0x17:    m_inst->RL_n(A); break;
    case 0x18:    m_inst->RR_n(B); break;
    case 0x19:    m_inst->RR_n(C); break;
    case 0x1A:    m_inst->RR_n(D); break;
    case 0x1B:    m_inst->RR_n(E); break;
    case 0x1C:    m_inst->RR_n(H); break;
    case 0x1D:    m_inst->RR_n(L); break;
    case 0x1E:    m_inst->RR_n(mHL); break;
    case 0x1f:    m_inst->RR_n(A); break;
    case 0x20:    m_inst->SLA_n(B); break;
    case 0x21:    m_inst->SLA_n(C); break;
    case 0x22:    m_inst->SLA_n(D); break;
    case 0x23:    m_inst->SLA_n(E); break;
    case 0x24:    m_inst->SLA_n(H); break;
    case 0x25:    m_inst->SLA_n(L); break;
    case 0x26:    m_inst->SLA_n(mHL); break;
    case 0x27:    m_inst->SLA_n(A); break;
    case 0x28:    m_inst->SRA_n(B); break;
    case 0x29:    m_inst->SRA_n(C); break;
    case 0x2A:    m_inst->SRA_n(D); break;
    case 0x2B:    m_inst->SRA_n(E); break;
    case 0x2C:    m_inst->SRA_n(H); break;
    case 0x2D:    m_inst->SRA_n(L); break;
    case 0x2E:    m_inst->SRA_n(mHL); break;
    case 0x2F:    m_inst->SRA_n(A); break;
    case 0x30:    m_inst->SWAP_n(B); break;
    case 0x31:    m_inst->SWAP_n(C); break;
    case 0x32:    m_inst->SWAP_n(D); break;
    case 0x33:    m_inst->SWAP_n(E); break;
    case 0x34:    m_inst->SWAP_n(H); break;
    case 0x35:    m_inst->SWAP_n(L); break;
    case 0x36:    m_inst->SWAP_n(mHL); break;
    case 0x37:    m_inst->SWAP_n(A); break;
    case 0x38:    m_inst->SRL_n(B); break;
    case 0x39:    m_inst->SRL_n(C); break;
    case 0x3A:    m_inst->SRL_n(D); break;
    case 0x3B:    m_inst->SRL_n(E); break;
    case 0x3C:    m_inst->SRL_n(H); break;
    case 0x3D:    m_inst->SRL_n(L); break;
    case 0x3E:    m_inst->SRL_n(mHL); break;
    case 0x3F:    m_inst->SRL_n(A); break;
    case 0x40:    m_inst->BIT_b_r(0, B); break;
    case 0x41:    m_inst->BIT_b_r(0, C); break;
    case 0x42:    m_inst->BIT_b_r(0, D); break;
    case 0x43:    m_inst->BIT_b_r(0, E); break;
    case 0x44:    m_inst->BIT_b_r(0, H); break;
    case 0x45:    m_inst->BIT_b_r(0, L); break;
    case 0x46:    m_inst->BIT_b_r(0, mHL); break;
    case 0x47:    m_inst->BIT_b_r(0, A); break;
    case 0x48:    m_inst->BIT_b_r(1, B); break;
    case 0x49:    m_inst->BIT_b_r(1, C); break;
    case 0x4A:    m_inst->BIT_b_r(1, D); break;
    case 0x4B:    m_inst->BIT_b_r(1, E); break;
    case 0x4C:    m_inst->BIT_b_r(1, H); break;
    case 0x4D:    m_inst->BIT_b_r(1, L); break;
    case 0x4E:    m_inst->BIT_b_r(1, mHL); break;
    case 0x4F:    m_inst->BIT_b_r(1, A); break;
    case 0x50:    m_inst->BIT_b_r(2, B); break;
    case 0x51:    m_inst->BIT_b_r(2, C); break;
    case 0x52:    m_inst->BIT_b_r(2, D); break;
    case 0x53:    m_inst->BIT_b_r(2, E); break;
    case 0x54:    m_inst->BIT_b_r(2, H); break;
    case 0x55:    m_inst->BIT_b_r(2, L); break;
    case 0x56:    m_inst->BIT_b_r(2, mHL); break;
    case 0x57:    m_inst->BIT_b_r(2, A); break;
    case 0x58:    m_inst->BIT_b_r(3, B); break;
    case 0x59:    m_inst->BIT_b_r(3, C); break;
    case 0x5A:    m_inst->BIT_b_r(3, D); break;
    case 0x5B:    m_inst->BIT_b_r(3, E); break;
    case 0x5C:    m_inst->BIT_b_r(3, H); break;
    case 0x5D:    m_inst->BIT_b_r(3, L); break;
    case 0x5E:    m_inst->BIT_b_r(3, mHL); break;
    case 0x5F:    m_inst->BIT_b_r(3, A); break;
    case 0x60:    m_inst->BIT_b_r(4, B); break;
    case 0x61:    m_inst->BIT_b_r(4, C); break;
    case 0x62:    m_inst->BIT_b_r(4, D); break;
    case 0x63:    m_inst->BIT_b_r(4, E); break;
    case 0x64:    m_inst->BIT_b_r(4, H); break;
    case 0x65:    m_inst->BIT_b_r(4, L); break;
    case 0x66:    m_inst->BIT_b_r(4, mHL); break;
    case 0x67:    m_inst->BIT_b_r(4, A); break;
    case 0x68:    m_inst->BIT_b_r(5, B); break;
    case 0x69:    m_inst->BIT_b_r(5, C); break;
    case 0x6A:    m_inst->BIT_b_r(5, D); break;
    case 0x6B:    m_inst->BIT_b_r(5, E); break;
    case 0x6C:    m_inst->BIT_b_r(5, H); break;
    case 0x6D:    m_inst->BIT_b_r(5, L); break;
    case 0x6E:    m_inst->BIT_b_r(5, mHL); break;
    case 0x6F:    m_inst->BIT_b_r(5, A); break;
    case 0x70:    m_inst->BIT_b_r(6, B); break;
    case 0x71:    m_inst->BIT_b_r(6, C); break;
    case 0x72:    m_inst->BIT_b_r(6, D); break;
    case 0x73:    m_inst->BIT_b_r(6, E); break;
    case 0x74:    m_inst->BIT_b_r(6, H); break;
    case 0x75:    m_inst->BIT_b_r(6, L); break;
    case 0x76:    m_inst->BIT_b_r(6, mHL); break;
    case 0x77:    m_inst->BIT_b_r(6, A); break;
    case 0x78:    m_inst->BIT_b_r(7, B); break;
    case 0x79:    m_inst->BIT_b_r(7, C); break;
    case 0x7A:    m_inst->BIT_b_r(7, D); break;
    case 0x7B:    m_inst->BIT_b_r(7, E); break;
    case 0x7C:    m_inst->BIT_b_r(7, H); break;
    case 0x7D:    m_inst->BIT_b_r(7, L); break;
    case 0x7E:    m_inst->BIT_b_r(7, mHL); break;
    case 0x7F:    m_inst->BIT_b_r(7, A); break;
    case 0x80:    m_inst->RES_b_r(0, B); break;
    case 0x81:    m_inst->RES_b_r(0, C); break;
    case 0x82:    m_inst->RES_b_r(0, D); break;
    case 0x83:    m_inst->RES_b_r(0, E); break;
    case 0x84:    m_inst->RES_b_r(0, H); break;
    case 0x85:    m_inst->RES_b_r(0, L); break;
    case 0x86:    m_inst->RES_b_r(0, mHL); break;
    case 0x87:    m_inst->RES_b_r(0, A); break;
    case 0x88:    m_inst->RES_b_r(1, B); break;
    case 0x89:    m_inst->RES_b_r(1, C); break;
    case 0x8A:    m_inst->RES_b_r(1, D); break;
    case 0x8B:    m_inst->RES_b_r(1, E); break;
    case 0x8C:    m_inst->RES_b_r(1, H); break;
    case 0x8D:    m_inst->RES_b_r(1, L); break;
    case 0x8E:    m_inst->RES_b_r(1, mHL); break;
    case 0x8F:    m_inst->RES_b_r(1, A); break;
    case 0x90:    m_inst->RES_b_r(2, B); break;
    case 0x91:    m_inst->RES_b_r(2, C); break;
    case 0x92:    m_inst->RES_b_r(2, D); break;
    case 0x93:    m_inst->RES_b_r(2, E); break;
    case 0x94:    m_inst->RES_b_r(2, H); break;
    case 0x95:    m_inst->RES_b_r(2, L); break;
    case 0x96:    m_inst->RES_b_r(2, mHL); break;
    case 0x97:    m_inst->RES_b_r(2, A); break;
    case 0x98:    m_inst->RES_b_r(3, B); break;
    case 0x99:    m_inst->RES_b_r(3, C); break;
    case 0x9A:    m_inst->RES_b_r(3, D); break;
    case 0x9B:    m_inst->RES_b_r(3, E); break;
    case 0x9C:    m_inst->RES_b_r(3, H); break;
    case 0x9D:    m_inst->RES_b_r(3, L); break;
    case 0x9E:    m_inst->RES_b_r(3, mHL); break;
    case 0x9F:    m_inst->RES_b_r(3, A); break;
    case 0xA0:    m_inst->RES_b_r(4, B); break;
    case 0xA1:    m_inst->RES_b_r(4, C); break;
    case 0xA2:    m_inst->RES_b_r(4, D); break;
    case 0xA3:    m_inst->RES_b_r(4, E); break;
    case 0xA4:    m_inst->RES_b_r(4, H); break;
    case 0xA5:    m_inst->RES_b_r(4, L); break;
    case 0xA6:    m_inst->RES_b_r(4, mHL); break;
    case 0xA7:    m_inst->RES_b_r(4, A); break;
    case 0xA8:    m_inst->RES_b_r(5, B); break;
    case 0xA9:    m_inst->RES_b_r(5, C); break;
    case 0xAA:    m_inst->RES_b_r(5, D); break;
    case 0xAB:    m_inst->RES_b_r(5, E); break;
    case 0xAC:    m_inst->RES_b_r(5, H); break;
    case 0xAD:    m_inst->RES_b_r(5, L); break;
    case 0xAE:    m_inst->RES_b_r(5, mHL); break;
    case 0xAF:    m_inst->RES_b_r(5, A); break;
    case 0xB0:    m_inst->RES_b_r(6, B); break;
    case 0xB1:    m_inst->RES_b_r(6, C); break;
    case 0xB2:    m_inst->RES_b_r(6, D); break;
    case 0xB3:    m_inst->RES_b_r(6, E); break;
    case 0xB4:    m_inst->RES_b_r(6, H); break;
    case 0xB5:    m_inst->RES_b_r(6, L); break;
    case 0xB6:    m_inst->RES_b_r(6, mHL); break;
    case 0xB7:    m_inst->RES_b_r(6, A); break;
    case 0xB8:    m_inst->RES_b_r(7, B); break;
    case 0xB9:    m_inst->RES_b_r(7, C); break;
    case 0xBA:    m_inst->RES_b_r(7, D); break;
    case 0xBB:    m_inst->RES_b_r(7, E); break;
    case 0xBC:    m_inst->RES_b_r(7, H); break;
    case 0xBD:    m_inst->RES_b_r(7, L); break;
    case 0xBE:    m_inst->RES_b_r(7, mHL); break;
    case 0xBF:    m_inst->RES_b_r(7, A); break;
    case 0xC0:    m_inst->SET_b_r(0, B); break;
    case 0xC1:    m_inst->SET_b_r(0, C); break;
    case 0xC2:    m_inst->SET_b_r(0, D); break;
    case 0xC3:    m_inst->SET_b_r(0, E); break;
    case 0xC4:    m_inst->SET_b_r(0, H); break;
    case 0xC5:    m_inst->SET_b_r(0, L); break;
    case 0xC6:    m_inst->SET_b_r(0, mHL); break;
    case 0xC7:    m_inst->SET_b_r(0, A); break;
    case 0xC8:    m_inst->SET_b_r(1, B); break;
    case 0xC9:    m_inst->SET_b_r(1, C); break;
    case 0xCA:    m_inst->SET_b_r(1, D); break;
    case 0xCB:    m_inst->SET_b_r(1, E); break;
    case 0xCC:    m_inst->SET_b_r(1, H); break;
    case 0xCD:    m_inst->SET_b_r(1, L); break;
    case 0xCE:    m_inst->SET_b_r(1, mHL); break;
    case 0xCF:    m_inst->SET_b_r(1, A); break;
    case 0xD0:    m_inst->SET_b_r(2, B); break;
    case 0xD1:    m_inst->SET_b_r(2, C); break;
    case 0xD2:    m_inst->SET_b_r(2, D); break;
    case 0xD3:    m_inst->SET_b_r(2, E); break;
    case 0xD4:    m_inst->SET_b_r(2, H); break;
    case 0xD5:    m_inst->SET_b_r(2, L); break;
    case 0xD6:    m_inst->SET_b_r(2, mHL); break;
    case 0xD7:    m_inst->SET_b_r(2, A); break;
    case 0xD8:    m_inst->SET_b_r(3, B); break;
    case 0xD9:    m_inst->SET_b_r(3, C); break;
    case 0xDA:    m_inst->SET_b_r(3, D); break;
    case 0xDB:    m_inst->SET_b_r(3, E); break;
    case 0xDC:    m_inst->SET_b_r(3, H); break;
    case 0xDD:    m_inst->SET_b_r(3, L); break;
    case 0xDE:    m_inst->SET_b_r(3, mHL); break;
    case 0xDF:    m_inst->SET_b_r(3, A); break;
    case 0xE0:    m_inst->SET_b_r(4, B); break;
    case 0xE1:    m_inst->SET_b_r(4, C); break;
    case 0xE2:    m_inst->SET_b_r(4, D); break;
    case 0xE3:    m_inst->SET_b_r(4, E); break;
    case 0xE4:    m_inst->SET_b_r(4, H); break;
    case 0xE5:    m_inst->SET_b_r(4, L); break;
    case 0xE6:    m_inst->SET_b_r(4, mHL); break;
    case 0xE7:    m_inst->SET_b_r(4, A); break;
    case 0xE8:    m_inst->SET_b_r(5, B); break;
    case 0xE9:    m_inst->SET_b_r(5, C); break;
    case 0xEA:    m_inst->SET_b_r(5, D); break;
    case 0xEB:    m_inst->SET_b_r(5, E); break;
    case 0xEC:    m_inst->SET_b_r(5, H); break;
    case 0xED:    m_inst->SET_b_r(5, L); break;
    case 0xEE:    m_inst->SET_b_r(5, mHL); break;
    case 0xEF:    m_inst->SET_b_r(5, A); break;
    case 0xF0:    m_inst->SET_b_r(6, B); break;
    case 0xF1:    m_inst->SET_b_r(6, C); break;
    case 0xF2:    m_inst->SET_b_r(6, D); break;
    case 0xF3:    m_inst->SET_b_r(6, E); break;
    case 0xF4:    m_inst->SET_b_r(6, H); break;
    case 0xF5:    m_inst->SET_b_r(6, L); break;
    case 0xF6:    m_inst->SET_b_r(6, mHL); break;
    case 0xF7:    m_inst->SET_b_r(6, A); break;
    case 0xF8:    m_inst->SET_b_r(7, B); break;
    case 0xF9:    m_inst->SET_b_r(7, C); break;
    case 0xFA:    m_inst->SET_b_r(7, D); break;
    case 0xFB:    m_inst->SET_b_r(7, E); break;
    case 0xFC:    m_inst->SET_b_r(7, H); break;
    case 0xFD:    m_inst->SET_b_r(7, L); break;
    case 0xFE:    m_inst->SET_b_r(7, mHL); break;
    case 0xFF:    m_inst->SET_b_r(7, A); break;

    default:
        cout << hex << "Unimplemented CB instruction \"" << (int)OPCODE << "\" at PC = "
            << m_registers.getPC() << dec << endl;
        return -1;
        break;
    }
    return m_CBinstCycles[OPCODE];
}

void CPU::initCyclesArrays()
{
    for (int i = 0x00; i <= 0xFF; i++) {
        m_instCycles[i] = 1;
        m_CBinstCycles[i] = 2;
    }

    // Instructions cycles
    m_instCycles[0x01] = 3;
    m_instCycles[0x02] = 2;
    m_instCycles[0x03] = 2;
    m_instCycles[0x06] = 2;
    m_instCycles[0x09] = 2;
    m_instCycles[0x0A] = 2;
    m_instCycles[0x0B] = 2;
    m_instCycles[0x0E] = 2;
    m_instCycles[0x11] = 3;
    m_instCycles[0x12] = 2;
    m_instCycles[0x13] = 2;
    m_instCycles[0x16] = 2;
    m_instCycles[0x18] = 3;
    m_instCycles[0x19] = 2;
    m_instCycles[0x1A] = 2;
    m_instCycles[0x1B] = 2;
    m_instCycles[0x1E] = 2;
    m_instCycles[0x20] = 2;
    m_instCycles[0x21] = 3;
    m_instCycles[0x22] = 2;
    m_instCycles[0x23] = 2;
    m_instCycles[0x26] = 2;
    m_instCycles[0x28] = 2;
    m_instCycles[0x29] = 2;
    m_instCycles[0x2A] = 2;
    m_instCycles[0x2B] = 2;
    m_instCycles[0x2E] = 2;
    m_instCycles[0x30] = 2;
    m_instCycles[0x31] = 3;
    m_instCycles[0x32] = 2;
    m_instCycles[0x33] = 2;
    m_instCycles[0x34] = 3;
    m_instCycles[0x35] = 3;
    m_instCycles[0x36] = 3;
    m_instCycles[0x38] = 2;
    m_instCycles[0x39] = 2;
    m_instCycles[0x3A] = 2;
    m_instCycles[0x3B] = 2;
    m_instCycles[0x3E] = 2;
    m_instCycles[0x46] = 2;
    m_instCycles[0x4E] = 2;
    m_instCycles[0x56] = 2;
    m_instCycles[0x5E] = 2;
    m_instCycles[0x66] = 2;
    m_instCycles[0x6E] = 2;
    m_instCycles[0x7E] = 2;
    m_instCycles[0x86] = 2;
    m_instCycles[0x8E] = 2;
    m_instCycles[0x96] = 2;
    m_instCycles[0x9E] = 2;
    m_instCycles[0xA6] = 2;
    m_instCycles[0xAE] = 2;
    m_instCycles[0xB6] = 2;
    m_instCycles[0xBE] = 2;
    m_instCycles[0xC0] = 2;
    m_instCycles[0xC1] = 3;
    m_instCycles[0xC2] = 3;
    m_instCycles[0xC3] = 4;
    m_instCycles[0xC4] = 3;
    m_instCycles[0xC5] = 4;
    m_instCycles[0xC6] = 2;
    m_instCycles[0xC8] = 2;
    m_instCycles[0xCE] = 2;
    m_instCycles[0xCF] = 4;
    m_instCycles[0xD0] = 2;
    m_instCycles[0xD1] = 3;
    m_instCycles[0xD2] = 3;
    m_instCycles[0xD4] = 3;
    m_instCycles[0xD5] = 4;
    m_instCycles[0xD6] = 2;
    m_instCycles[0xD7] = 4;
    m_instCycles[0xD8] = 2;
    m_instCycles[0xD9] = 4;
    m_instCycles[0xDA] = 3;
    m_instCycles[0xDC] = 3;
    m_instCycles[0xDE] = 2;
    m_instCycles[0xDF] = 4;
    m_instCycles[0xE6] = 2;
    m_instCycles[0xE0] = 3;
    m_instCycles[0xE1] = 3;
    m_instCycles[0xE2] = 2;
    m_instCycles[0xE5] = 4;
    m_instCycles[0xE6] = 2;
    m_instCycles[0xE7] = 4;
    m_instCycles[0xE8] = 4;
    m_instCycles[0xEA] = 4;
    m_instCycles[0xEE] = 2;
    m_instCycles[0xEF] = 4;
    m_instCycles[0xF0] = 3;
    m_instCycles[0xF1] = 3;
    m_instCycles[0xF2] = 2;
    m_instCycles[0xF5] = 4;
    m_instCycles[0xF6] = 2;
    m_instCycles[0xF7] = 4;
    m_instCycles[0xF8] = 3;
    m_instCycles[0xF9] = 2;
    m_instCycles[0xFA] = 4;
    m_instCycles[0xFE] = 2;
    m_instCycles[0xFF] = 4;

    // CB prefix instructions cycles
    m_CBinstCycles[0x06] = 4;
    m_CBinstCycles[0x0E] = 4;
    m_CBinstCycles[0x16] = 4;
    m_CBinstCycles[0x1E] = 4;
    m_CBinstCycles[0x26] = 4;
    m_CBinstCycles[0x2E] = 4;
    m_CBinstCycles[0x36] = 4;
    m_CBinstCycles[0x3E] = 4;
    m_CBinstCycles[0x46] = 4;
    m_CBinstCycles[0x4E] = 4;
    m_CBinstCycles[0x56] = 4;
    m_CBinstCycles[0x5E] = 4;
    m_CBinstCycles[0x66] = 4;
    m_CBinstCycles[0x6E] = 4;
    m_CBinstCycles[0x76] = 4;
    m_CBinstCycles[0x7E] = 4;
    m_CBinstCycles[0x86] = 4;
    m_CBinstCycles[0x8E] = 4;
    m_CBinstCycles[0x96] = 4;
    m_CBinstCycles[0x9E] = 4;
    m_CBinstCycles[0xA6] = 4;
    m_CBinstCycles[0xAE] = 4;
    m_CBinstCycles[0xB6] = 4;
    m_CBinstCycles[0xBE] = 4;
    m_CBinstCycles[0xC6] = 4;
    m_CBinstCycles[0xCE] = 4;
    m_CBinstCycles[0xD6] = 4;
    m_CBinstCycles[0xDE] = 4;
    m_CBinstCycles[0xE6] = 4;
    m_CBinstCycles[0xEE] = 4;
    m_CBinstCycles[0xF6] = 4;
    m_CBinstCycles[0xFE] = 4;
}

void CPU::condCycles()
{
    m_instCycles[0x20] = 3;
    m_instCycles[0x28] = 3;
    m_instCycles[0x30] = 3;
    m_instCycles[0x38] = 3;
    m_instCycles[0xC0] = 5;
    m_instCycles[0xC2] = 4;
    m_instCycles[0xC4] = 6;
    m_instCycles[0xC8] = 5;
    m_instCycles[0xCA] = 4;
    m_instCycles[0xCC] = 6;
    m_instCycles[0xD0] = 5;
    m_instCycles[0xD2] = 4;
    m_instCycles[0xD4] = 6;
    m_instCycles[0xD8] = 5;
    m_instCycles[0xDA] = 4;
    m_instCycles[0xDC] = 6;
}

void CPU::uncondCycles()
{
    m_instCycles[0x20] = 2;
    m_instCycles[0x28] = 2;
    m_instCycles[0x30] = 2;
    m_instCycles[0x38] = 2;
    m_instCycles[0xC0] = 2;
    m_instCycles[0xC2] = 3;
    m_instCycles[0xC4] = 3;
    m_instCycles[0xC8] = 2;
    m_instCycles[0xCA] = 3;
    m_instCycles[0xCC] = 3;
    m_instCycles[0xD0] = 2;
    m_instCycles[0xD2] = 3;
    m_instCycles[0xD4] = 3;
    m_instCycles[0xD8] = 2;
    m_instCycles[0xDA] = 3;
    m_instCycles[0xDC] = 3;
}

void CPU::requestInterrupt(Interrupt id)
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

    BYTE reqInt = m_memory->read(IF);
    //cout << "REQ_BEF: " << hex << (int)reqInt << endl;
    reqInt = bitSet(reqInt, id);
    m_memory->write(IF, reqInt);

    if (testBit(m_memory->read(IE), id)) {
        m_registers.setHalt(false);
    }
    //cout << "REQ_AFT: " << hex << (int)reqInt << endl;
    //cout << "ENAB: " << hex << (int)_memory->read(0xFFFF) << endl;
}

void CPU::doInterrupts()
{
    if (!m_IME) { return; }

    BYTE reqInt = m_memory->read(IF);
    BYTE enabInt = m_memory->read(IE);

    //cout << "REQ: " << hex << (int)reqInt << endl;
    //cout << "ENAB: " << hex << (int)enabInt << endl;

    if (reqInt == 0x00) { return; }

    // Check each possible interrupt
    for (int i = 0; i < 5; i++) {
        if (testBit(reqInt, i) && testBit(enabInt, i)) {
            serviceInterrupt(static_cast<Interrupt>(i));
        }
    }
}

void CPU::serviceInterrupt(Interrupt id)
{
    //cout << "Interrupt Serviced" << endl;
    m_IME = false;

    // Unset interrupt bit
    BYTE reqInt = m_memory->read(IF);
    reqInt = bitReset(reqInt, id);
    m_memory->write(IF, reqInt);

    m_registers.setSP(m_registers.getSP() - 1);
    m_memory->write(m_registers.getSP(), (m_registers.getPC() & 0xFF00) >> 8);
    m_registers.setSP(m_registers.getSP() - 1);
    m_memory->write(m_registers.getSP(), m_registers.getPC() & 0x00FF);

    /*switch (id)
    {
    case VBlank: cout << "VBLANK serviced" << endl; break;
    case LCD: cout << "LCD serviced" << endl; break;
    case Timer: cout << "Timer serviced" << endl; break;
    case JoyPad: cout << "JoyPad Serviced" << endl; break;
    default:
        break;
    }*/

    m_registers.setHalt(false);

    switch (id) {
    case VBlank: m_registers.setPC(0x40); break;
    case LCD: m_registers.setPC(0x48); break;
    case Timer: m_registers.setPC(0x50); break;
    case CLink: break; // Cable Link
    case JoyPad: m_registers.setPC(0x60); break;
    }
}

void CPU::updateTimers(int cycles)
{
    //cout << m_divideCounter << endl;
    
    m_divideCounter += cycles;
    while (m_divideCounter >= 64) {
        m_divideCounter -= 64;
        m_memory->directModification(DIV, m_memory->read(DIV) + 1);
    }

    if (!isClockEnabled()) {
        return;
    }
    
    BYTE freq = getClockFreq();

    m_timeCounter += cycles;

    while (m_timeCounter >= freq) {
        m_timeCounter -= freq;
        if (m_memory->read(TIMA) == 255) {
            m_memory->write(TIMA, m_memory->read(TMA));
            requestInterrupt(Timer);
        } else {
            m_memory->write(TIMA, m_memory->read(TIMA) + 1);
        }
    }
}

BYTE CPU::getClockFreq() const
{
    BYTE freq = m_memory->read(TAC) & 0x3;
    switch (freq) {
    case 0: return 256;    // freq 4096
    case 1: return 4;      // freq 262144
    case 2: return 16;     // freq 65536
    case 3: return 64;     // freq 16382
    }
}

bool CPU::isClockEnabled()
{
    //cout << hex << (int)_memory->read(TAC) << endl;
    return testBit(m_memory->read(TAC), 2) ? true : false;
}
