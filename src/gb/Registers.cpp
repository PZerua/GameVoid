#include "Registers.h"

Registers::Registers()
{
    reset();
}

Registers::~Registers()
{

}

// Set initial values
void Registers::reset()
{
    AF.nn = 0x11B0;
    BC.nn = 0x0013;
    DE.nn = 0x00D8;
    HL.nn = 0x014D;
    SP = 0xFFFE;
    PC = 0x0100;
    m_haltEnabled = false;
}

// Get A from AF (higher 8 bits)
const BYTE Registers::getA() { return AF.n[1]; }

// Get F from AF (lower 8 bits)
const BYTE Registers::getF() { return AF.n[0]; }

// Get Z flag
const BYTE Registers::getF_Z() { return (getF() & 0x80) >> 7; }

// Get N flag
const BYTE Registers::getF_N() { return (getF() & 0x40) >> 6; }

// Get H flag
const BYTE Registers::getF_H() { return (getF() & 0x20) >> 5; }

// Get C flag
const BYTE Registers::getF_C() { return (getF() & 0x10) >> 4; }

// Get B from BC (higher 8 bits)
const BYTE Registers::getB() { return BC.n[1]; }

// Get C from BC (lower 8 bits)
const BYTE Registers::getC() { return BC.n[0]; }

// Get D from DE (higher 8 bits)
const BYTE Registers::getD() { return DE.n[1]; }

// Get E from DE (lower 8 bits)
const BYTE Registers::getE() { return DE.n[0]; }

// Get H from HL (higher 8 bits)
const BYTE Registers::getH() { return HL.n[1]; }

// Get L from HL (lower 8 bits)
const BYTE Registers::getL() { return HL.n[0]; }

const WORD Registers::getAF() { return AF.nn; }

const WORD Registers::getBC() { return BC.nn; }

const WORD Registers::getDE() { return DE.nn; }

const WORD Registers::getHL() { return HL.nn; }

const WORD Registers::getPC() { return PC; }

const WORD Registers::getSP() { return SP; }

// Zero Flag
void Registers::setF_Z(BYTE value)
{
    if ( value == 0x00 || value == 0x01)
        AF.n[0] = (AF.n[0] & 0x7F) | (value << 7);
    else throw exception("Value should always be 0 or 1");
}

// Subtract Flag
void Registers::setF_N(BYTE value)
{
    if (value == 0x00 || value == 0x01)
        AF.n[0] = (AF.n[0] & 0xBF) | (value << 6);
    else throw exception("Value should always be 0 or 1");
}

// Half Carry Flag
void Registers::setF_H(BYTE value)
{
    if (value == 0x00 || value == 0x01)
        AF.n[0] = (AF.n[0] & 0xDF) | (value << 5);
    else throw exception("Value should always be 0 or 1");
}

// Carry Flag
void Registers::setF_C(BYTE value)
{
    if (value == 0x00 || value == 0x01)
        AF.n[0] = (AF.n[0] & 0xEF) | (value << 4);
    else throw exception("Value should always be 0 or 1");
}

// Set higher 8 bits of AF
void Registers::setA(BYTE value) { AF.n[1] = value; }

// Set lower 8 bits of AF
void Registers::setF(BYTE value) { AF.n[0] = value & 0xF0; }

// Set higher 8 bits of BC
void Registers::setB(BYTE value) { BC.n[1] = value; }

// Set lower 8 bits of BC
void Registers::setC(BYTE value) { BC.n[0] = value; }

// Set higher 8 bits of DE
void Registers::setD(BYTE value) { DE.n[1] = value; }

// Set lower 8 bits of DE
void Registers::setE(BYTE value) { DE.n[0] = value; }

// Set higher 8 bits of HL
void Registers::setH(BYTE value) { HL.n[1] = value; }

// Set lower 8 bits of HL
void Registers::setL(BYTE value) { HL.n[0] = value; }

void Registers::setAF(WORD value) { AF.nn = value & 0xFFF0; }

void Registers::setBC(WORD value) { BC.nn = value; }

void Registers::setDE(WORD value) { DE.nn = value; }

void Registers::setHL(WORD value) { HL.nn = value; }

void Registers::setSP(WORD value) { SP = value; }

void Registers::addPC(int value) { PC += value; }

void Registers::addSP(int value) { SP += value; }

void Registers::setPC(WORD value) { PC = value; }

void Registers::setReg(regID id, WORD value)
{
    switch (id)
    {
    case A: setA((BYTE)value); break;
    case B: setB((BYTE)value); break;
    case C: setC((BYTE)value); break;
    case D: setD((BYTE)value); break;
    case E: setE((BYTE)value); break;
    case F: setF((BYTE)value); break;
    case H: setH((BYTE)value); break;
    case L: setL((BYTE)value); break;
    case af: setAF(value); break;
    case bc: setBC(value); break;
    case de: setDE(value); break;
    case hl: setHL(value); break;
    case sp: setSP(value); break;
    default:
        throw exception("SetReg error, unimplemented enum");
        break;
    }
}

const WORD Registers::getReg(regID id)
{
    switch (id)
    {
    case A: return getA();
    case B: return getB();
    case C: return getC();
    case D: return getD();
    case E: return getE();
    case F: return getF();
    case H: return getH();
    case L: return getL();
    case af: return getAF();
    case bc: return getBC();
    case de: return getDE();
    case hl: return getHL();
    case sp: return getSP();
    default:
        throw exception("GetReg error, unimplemented enum");
        break;
    }
}

void Registers::setHalt(bool value)
{
    m_haltEnabled = value;
}

bool Registers::haltEnabled()
{
    return m_haltEnabled;
}