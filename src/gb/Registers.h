#pragma once

#include "Header.h"

enum regID
{
    A, B, C, D, E, F, H, L,
    af, bc, de, hl, sp, pc,
    n8, n16, mBC, mDE, mHL,
    nZ, Z, nC, sC
};

class Registers
{
public:
    Registers();
    ~Registers();

    void reset();

    const BYTE getA();
    const BYTE getF();
    const BYTE getF_Z();
    const BYTE getF_N();
    const BYTE getF_H();
    const BYTE getF_C();
    const BYTE getB();
    const BYTE getC();
    const BYTE getD();
    const BYTE getE();
    const BYTE getH();
    const BYTE getL();
    const WORD getAF();
    const WORD getBC();
    const WORD getDE();
    const WORD getHL();
    const WORD getPC();
    const WORD getSP();

    void setA(BYTE value);
    void setF(BYTE value);
    void setF_Z(BYTE value);
    void setF_N(BYTE value);
    void setF_H(BYTE value);
    void setF_C(BYTE value);
    void setB(BYTE value);
    void setC(BYTE value);
    void setD(BYTE value);
    void setE(BYTE value);
    void setH(BYTE value);
    void setL(BYTE value);
    void setAF(WORD value);
    void setBC(WORD value);
    void setDE(WORD value);
    void setHL(WORD value);
    void addPC(int value);
    void addSP(int value);
    void setSP(WORD value);
    void setPC(WORD value);

    void setReg(regID id, WORD value);
    const WORD getReg(regID id);

    void setHalt(bool value);
    bool haltEnabled();

private:

    union reg
    {
        WORD nn;
        BYTE n[2];
    };

    reg AF;
    reg BC;
    reg DE;
    reg HL;
    // Stack Pointer
    WORD SP;
    // Program Counter
    WORD PC;

    bool _haltEnabled;
};
