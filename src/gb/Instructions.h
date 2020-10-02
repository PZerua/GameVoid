#pragma once

#include "data_types.h"
#include "cpu_registers.h"
#include "memory.h"

class Instructions
{
public:
    Instructions(Memory *memory, Registers *registers);

    WORD read16();
    BYTE read8();
    bool hasCarry8(int a, int b);
    bool hasHalfCarry8(int a, int b);
    bool hasCarry16(int a, int b);
    bool hasHalfCarry16(int a, int b);
    bool hasBorrow8(int a, int b);
    bool hasHalfBorrow8(int a, int b);
    bool hasBorrow16(int a, int b);
    bool hasHalfBorrow16(int a, int b);
    void NOP();
    void LD_nn_n(regID nn);
    void LD_n_nn(regID n);
    void LD_n_A(regID n);
    void LD_A_n(regID n);
    void LD_nn_SP();
    void LD_HLI_A();
    void LD_HLD_A();
    void LD_A_HLI();
    void LD_A_HLD();
    void LD_r1_r2(regID r1, regID r2);
    void LD_C_A();
    void LD_A_C();
    void LD_SP_HL();
    void LDHL_SP_n();
    void LDH_n_A();
    void LDH_A_n();
    void ADD_HL_n(regID n);
    void ADD_A_n( regID n);
    void ADD_SP_n();
    void ADC_A_n(regID n);
    void INC_nn(regID n);
    void INC_n(regID n);
    void SUB_A_n(regID n);
    void SBC_A_n(regID n);
    void DEC_nn(regID nn);
    void DEC_n(regID n);
    void RLCA();
    void RLA();
    void RRCA();
    void RRA();
    void STOP();
    void JR_n();
    void JR_cc_n(regID n, bool &condTaken);
    void JP_cc_nn(regID cc, bool &condTaken);
    void DAA();
    void CPL();
    void HALT(bool IME);
    void JP_nn();
    void JP_HL();
    void AND_n(regID n);
    void XOR_n(regID n);
    void OR_n(regID n);
    void CP_n(regID n);
    void RET_cc(regID cc, bool &condTaken);
    void RET();
    void RETI(bool &IME);
    void POP_nn(regID nn);
    void CALL_nn();
    void CALL_cc_nn(regID cc, bool &condTaken);
    void DI(bool &IME);
    void EI(bool &IME);
    void SCF();
    void CCF();
    void PUSH_nn(regID nn);
    void RST_n(BYTE n);

    void RLC_n(regID n);
    void RRC_n(regID n);
    void SWAP_n(regID n);
    void BIT_b_r(BYTE b, regID r);
    void RES_b_r(BYTE b, regID r);
    void SLA_n(regID n);
    void SRA_n(regID n);
    void SRL_n(regID n);
    void SET_b_r(BYTE b, regID r);
    void RR_n(regID n);
    void RL_n(regID n);

private:
    Memory*     m_memory;
    Registers*  m_registers;
};
