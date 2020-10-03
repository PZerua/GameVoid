#include "instructions.h"

#include <iostream>

#include "memory.h"

using namespace std;

Instructions::Instructions(Memory *memory, Registers *registers)
{
    m_memory = memory;
    m_registers = registers;
}

// Read immediate 16 bits value
WORD Instructions::read16()
{
    BYTE low = m_memory->read(m_registers->getPC() + 1);
    BYTE high = m_memory->read(m_registers->getPC() + 2);
    WORD address = low + (high << 8);

    return address;
}

// Read immediate 8 bits value
BYTE Instructions::read8()
{
    return m_memory->read(m_registers->getPC() + 1);
}

bool Instructions::hasHalfCarry8(int a, int b)
{
    if ((((a & 0x000F) + (b & 0x000F)) & 0x10) == 0x10)
        return true;
    else return false;
}

bool Instructions::hasCarry8(int a, int b)
{
    if ((((a & 0x00FF) + (b & 0x00FF)) & 0x100) == 0x100)
        return true;
    else return false;
}

bool Instructions::hasHalfCarry16(int a, int b)
{
    if ((((a & 0x0FFF) + (b & 0x0FFF)) & 0x1000) == 0x1000)
        return true;
    else return false;
}

bool Instructions::hasCarry16(int a, int b)
{
    if ((((a & 0xFFFF) + (b & 0xFFFF)) & 0x10000) == 0x10000)
        return true;
    else return false;
}

bool Instructions::hasHalfBorrow8(int a, int b)
{
    if ((((a & 0x000F) - (b & 0x000F)) & 0x10) == 0x10)
        return true;
    else return false;
}

bool Instructions::hasBorrow8(int a, int b)
{
    if ((((a & 0x00FF) - (b & 0x00FF)) & 0x100) == 0x100)
        return true;
    else return false;
}

bool Instructions::hasHalfBorrow16(int a, int b)
{
    if ((((a & 0x0FFF) - (b & 0x0FFF)) & 0x1000) == 0x1000)
        return true;
    else return false;
}

bool Instructions::hasBorrow16(int a, int b)
{
    if ((((a & 0xFFFF) - (b & 0xFFFF)) & 0x10000) == 0x10000)
        return true;
    else return false;
}

// 0x00
void Instructions::NOP()
{
    m_registers->addPC(1);
}

// 0x01, 0x11, 0x21, 0x31
// n = BC, DE, HL, SP
// nn = 16 bit value
void Instructions::LD_n_nn(regID n)
{
    // nn = read16()
    m_registers->setReg(n, read16());
    m_registers->addPC(3);
}

// 0x02, 0x12, 0x47, 0x4F, 0x57, 0x5F, 0x67, 0x6F, 0x77, 0x7F, 0xEA
// n = A, B, C, D, E, H, L, (BC), (DE), (HL), (nn, inm 16)
void Instructions::LD_n_A(regID n)
{
    switch (n)
    {
    case mBC:
        m_memory->write(m_registers->getBC(), m_registers->getA());
        break;
    case mDE:
        m_memory->write(m_registers->getDE(), m_registers->getA());
        break;
    case mHL:
        m_memory->write(m_registers->getHL(), m_registers->getA());
        break;
    case n16:
        m_memory->write(read16(), m_registers->getA());
        break;
    default:
        // For n = A, B, C, D, E, H, L 
        m_registers->setReg(n, m_registers->getA());
        break;
    }

    if (n != n16)
        m_registers->addPC(1);
    else m_registers->addPC(3);
}

// 0x03, 0x13, 0x23, 0x33
// nn = BC, DE, HL, SP
void Instructions::INC_nn(regID nn)
{
    m_registers->setReg(nn, m_registers->getReg(nn) + 1);
    m_registers->addPC(1);
}

// 0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C, 0x34, 0x3C
// n = A, B, C, D, E, H, L, (HL)
void Instructions::INC_n(regID n)
{
    switch (n)
    {
    case mHL:
        m_registers->setF_H(hasHalfCarry8(m_memory->read(m_registers->getHL()), 1));
        m_memory->write(m_registers->getHL(), m_memory->read(m_registers->getHL()) + 1);
        m_registers->setF_Z(m_memory->read(m_registers->getHL()) == 0);
        break;
    default:
        // Set carry in flag
        m_registers->setF_H(hasHalfCarry8(m_registers->getReg(n), 1));
        m_registers->setReg(n, m_registers->getReg(n) + 1);
        m_registers->setF_Z(m_registers->getReg(n) == 0);
        break;
    }

    // Set N in flag
    m_registers->setF_N(0);

    m_registers->addPC(1);
}

// 0x05, 0x0D, 0x15, 0x1D, 0x25, 0x2D, 0x35, 0x3D
void Instructions::DEC_n(regID n)
{
    // Set carry in flag
    if (n != mHL) 
        m_registers->setF_H(hasHalfBorrow8(m_registers->getReg(n), 1));
    else m_registers->setF_H(hasHalfBorrow8(m_memory->read(m_registers->getHL()), 1));

    switch (n)
    {
    case mHL:
        m_memory->write(m_registers->getHL(), m_memory->read(m_registers->getHL()) - 1);
        break;
    default:
        m_registers->setReg(n, m_registers->getReg(n) - 1);
        break;
    }

    // Set Z and N in flag
    if (n != mHL) 
        m_registers->setF_Z(m_registers->getReg(n) == 0);
    else m_registers->setF_Z(m_memory->read(m_registers->getHL()) == 0);
    m_registers->setF_N(1);

    m_registers->addPC(1);
}

// 0x06, 0x0E, 0x16, 0x1E, 0x26, 0x2E
// nn = B, C, D, E, H, L
// n = immediate 8 bits
void Instructions::LD_nn_n(regID nn)
{
    m_registers->setReg(nn, read8());
    m_registers->addPC(2);
}

// 0x07
void Instructions::RLCA()
{
    // Set bit 7 as carry
    m_registers->setF_C((m_registers->getA() & 0x80) >> 7);
    // Rotate
    m_registers->setA(((m_registers->getA() << 1) & 0xFE) + m_registers->getF_C());
    // Set flag values
    m_registers->setF_N(0);
    m_registers->setF_H(0);
    m_registers->setF_Z(0);

    m_registers->addPC(1);
}

//0x08
// nn = 16 bits address
void Instructions::LD_nn_SP()
{
    WORD destAdress = read16();
    m_memory->write(destAdress, m_registers->getSP() & 0x00FF);
    m_memory->write(destAdress + 1, m_registers->getSP() >> 8);
    m_registers->addPC(3);
}

// 0x09, 0x19, 0x29, 0x39
void Instructions::ADD_HL_n(regID n)
{
    // Set carries in flag
    m_registers->setF_C(hasCarry16(m_registers->getHL(), m_registers->getReg(n)));
    m_registers->setF_H(hasHalfCarry16(m_registers->getHL(), m_registers->getReg(n)));

    // Add values
    m_registers->setHL(m_registers->getHL() + m_registers->getReg(n));

    // Reset flag value N
    m_registers->setF_N(0);

    m_registers->addPC(1);
}

// 0x0A, 0x1A, 0x3E, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, 0xFA
void Instructions::LD_A_n(regID n)
{
    switch (n)
    {
    case mBC:
        m_registers->setA(m_memory->read(m_registers->getBC()));
        break;
    case mDE:
        m_registers->setA(m_memory->read(m_registers->getDE()));
        break;
    case mHL:
        m_registers->setA(m_memory->read(m_registers->getHL()));
        break;
    case n8:
        m_registers->setA(read8());
        break;
    case n16:
        m_registers->setA(m_memory->read(read16()));
        break;
    default:
        m_registers->setA((BYTE)m_registers->getReg(n));
        break;
    }

    if (n == n8)
        m_registers->addPC(2);
    else if (n != n16)
        m_registers->addPC(1);
    else m_registers->addPC(3);
}

// 0x0B
void Instructions::DEC_nn(regID nn)
{
    m_registers->setReg(nn, m_registers->getReg(nn) - 1);
    m_registers->addPC(1);
}

// 0x0F
void Instructions::RRCA()
{
    // Set bit 0 as carry
    m_registers->setF_C(m_registers->getA() & 0x01);
    // Rotate
    m_registers->setA(((m_registers->getA() >> 1) & 0x7F) + (m_registers->getF_C() << 7));
    // Set flag values
    m_registers->setF_N(0);
    m_registers->setF_H(0);
    m_registers->setF_Z(0);

    m_registers->addPC(1);
}

// 0x10
void Instructions::STOP()
{
    // TODO
    m_registers->addPC(2);
}

// 0x17
void Instructions::RLA()
{
    // Save previous carry
    BYTE prevC = (m_registers->getF() & 0x10) >> 4;
    // Set bit 7 as carry
    m_registers->setF_C((m_registers->getA() & 0x80) >> 7);
    // Rotate
    m_registers->setA(((m_registers->getA() << 1) & 0xFE) + prevC);
    // Set flag values
    m_registers->setF_N(0);
    m_registers->setF_H(0);
    m_registers->setF_Z(0);

    m_registers->addPC(1);
}

// 0x18
void Instructions::JR_n()
{
    // Signed value
    char address;

    address = read8();

    m_registers->addPC(2 + address);
}

// 0x1F
void Instructions::RRA()
{
    // Save previous carry
    BYTE prevC = (m_registers->getF() & 0x10) >> 4;
    // Set bit 0 as carry
    m_registers->setF_C(m_registers->getA() & 0x01);
    // Rotate
    m_registers->setA(((m_registers->getA() >> 1) & 0x7F) + (prevC << 7));
    // Set flag values
    m_registers->setF_N(0);
    m_registers->setF_H(0);
    m_registers->setF_Z(0);

    m_registers->addPC(1);
}

// 0x20, 0x28, 0x30, 0x38
void Instructions::JR_cc_n(regID n, bool &condTaken)
{
    switch (n)
    {
    case nZ:
        if (m_registers->getF_Z() == 0)
        {
            JR_n();
            condTaken = true;
        }
        else
        {
            m_registers->addPC(2);
            condTaken = false;
        }
        break;
    case Z:
        if (m_registers->getF_Z() == 1)
        {
            JR_n();
            condTaken = true;
        }
        else
        {
            m_registers->addPC(2);
            condTaken = false;
        }
        break;
    case nC:
        if (m_registers->getF_C() == 0)
        {
            JR_n();
            condTaken = true;
        }
        else
        {
            m_registers->addPC(2);
            condTaken = false;
        }
        break;
    case sC:
        if (m_registers->getF_C() == 1)
        {
            JR_n();
            condTaken = true;
        }
        else
        {
            m_registers->addPC(2);
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
    m_memory->write(m_registers->getHL(), m_registers->getA());
    // Increments HL
    m_registers->setHL(m_registers->getHL() + 1);

    m_registers->addPC(1);
}

// 0x27
void Instructions::DAA()
{
    int a = m_registers->getA();

    if (m_registers->getF_N() == 0)
    {
        if (m_registers->getF_H() || ((a & 0xF) > 9))
            a += 0x06;

        if (m_registers->getF_C() || (a > 0x9F))
            a += 0x60;
    }
    else
    {
        if (m_registers->getF_H())
            a = (a - 6) & 0xFF;

        if (m_registers->getF_C())
            a -= 0x60;
    }

    m_registers->setF_H(0);
    m_registers->setF_Z(0);

    if ((a & 0x100) == 0x100)
        m_registers->setF_C(1);

    a &= 0xFF;

    if (a == 0)
        m_registers->setF_Z(1);

    m_registers->setA(a);

    m_registers->addPC(1);
}

// 0x2A
void Instructions::LD_A_HLI()
{
    // Loads HL address value into A
    m_registers->setA(m_memory->read(m_registers->getHL()));
    // Increment HL
    m_registers->setHL(m_registers->getHL() + 1);

    m_registers->addPC(1);
}

// 0x2F
void Instructions::CPL()
{
    m_registers->setA(~m_registers->getA());

    m_registers->setF_N(1);
    m_registers->setF_H(1);

    m_registers->addPC(1);
}

// 0x32
void Instructions::LD_HLD_A()
{
    // Load A in address contained in HL
    m_memory->write(m_registers->getHL(), m_registers->getA());
    // Increments HL
    m_registers->setHL(m_registers->getHL() - 1);

    m_registers->addPC(1);
}

// 0x36, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x48, 0x49, 0x4A, 0x4B
// 0x4C, 0x4D, 0x4E, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x58, 0x59
// 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66
// 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x70, 0x71, 0x72, 0x73, 0x74
// 0x75, 0x76, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E
void Instructions::LD_r1_r2(regID r1, regID r2)
{
    if (r1 == mHL)
    {
        if (r2 != n8)
        {
            m_memory->write(m_registers->getHL(), (BYTE)m_registers->getReg(r2));
            m_registers->addPC(1);
        }
        else 
        {
            m_memory->write(m_registers->getHL(), read8());
            m_registers->addPC(2);
        }
    }
    else
    {
        if (r2 != mHL)
            m_registers->setReg(r1, m_registers->getReg(r2));
        else m_registers->setReg(r1, m_memory->read(m_registers->getHL()));
        m_registers->addPC(1);
    }
}

// 0x37
void Instructions::SCF()
{
    m_registers->setF_N(0);
    m_registers->setF_H(0);
    m_registers->setF_C(1);
    m_registers->addPC(1);
}

// 0x3A
void Instructions::LD_A_HLD()
{
    // Loads HL address value into A
    m_registers->setA(m_memory->read(m_registers->getHL()));
    // Increment HL
    m_registers->setHL(m_registers->getHL() - 1);

    m_registers->addPC(1);
}

// 0x3F
void Instructions::CCF()
{
    m_registers->setF_N(0);
    m_registers->setF_H(0);
    m_registers->setF_C(!m_registers->getF_C());
    m_registers->addPC(1);
}

// 0x76
void Instructions::HALT(bool IME)
{
    //if (!IME) {
        m_registers->addPC(1);
    //}
}

// 0x80, 0x81, 0x82, 0x83, 0x84, 0x85 0x86, 0x87, 0xC6
void Instructions::ADD_A_n(regID n)
{
    switch (n)
    {
    case mHL:
        m_registers->setF_H(hasHalfCarry8(m_registers->getA(), m_memory->read(m_registers->getHL())));
        m_registers->setF_C(hasCarry8(m_registers->getA(), m_memory->read(m_registers->getHL())));
        m_registers->setA(m_registers->getA() + m_memory->read(m_registers->getHL()));
        break;
    case n8:
        m_registers->setF_H(hasHalfCarry8(m_registers->getA(), read8()));
        m_registers->setF_C(hasCarry8(m_registers->getA(), read8()));
        m_registers->setA(m_registers->getA() + read8());
        break;
    default:
        m_registers->setF_H(hasHalfCarry8(m_registers->getA(), m_registers->getReg(n)));
        m_registers->setF_C(hasCarry8(m_registers->getA(), m_registers->getReg(n)));
        m_registers->setA(m_registers->getA() + m_registers->getReg(n));
        break;
    }
    // Set flags
    m_registers->setF_Z(m_registers->getA() == 0);
    m_registers->setF_N(0);
    
    if (n != n8)
        m_registers->addPC(1);
    else m_registers->addPC(2);
}

// 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0xCE
void Instructions::ADC_A_n(regID n)
{
    int value, length = 1;
    BYTE valueReg;

    switch (n)
    {
    case n8:
        valueReg = read8(); length = 2; break;
    case mHL:
        valueReg = m_memory->read(m_registers->getHL()); break;
    default:
        valueReg = (BYTE)m_registers->getReg(n); break;
    }

    value = m_registers->getF_C() + valueReg + m_registers->getA();

    m_registers->setF_Z(!(value & 0xFF) ? 1 : 0);
    m_registers->setF_N(0);
    if ((m_registers->getF_C() + (valueReg & 0x0F) + (m_registers->getA() & 0x0F)) > 0x0F)
        m_registers->setF_H(1);
    else
        m_registers->setF_H(0);
    m_registers->setF_C((value > 0xFF) ? 1 : 0);

    m_registers->setA(value & 0xFF);

    m_registers->addPC(length);
}

// 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0xD6
void Instructions::SUB_A_n(regID n)
{
    switch (n)
    {
    case mHL:
        m_registers->setF_H(hasHalfBorrow8(m_registers->getA(), m_memory->read(m_registers->getHL())));
        m_registers->setF_C(hasBorrow8(m_registers->getA(), m_memory->read(m_registers->getHL())));
        m_registers->setA(m_registers->getA() - m_memory->read(m_registers->getHL()));
        break;
    case n8:
        m_registers->setF_H(hasHalfBorrow8(m_registers->getA(), read8()));
        m_registers->setF_C(hasBorrow8(m_registers->getA(), read8()));
        m_registers->setA(m_registers->getA() - read8());
        break;
    default:
        m_registers->setF_H(hasHalfBorrow8(m_registers->getA(), m_registers->getReg(n)));
        m_registers->setF_C(hasBorrow8(m_registers->getA(), m_registers->getReg(n)));
        m_registers->setA(m_registers->getA() - m_registers->getReg(n));
        break;
    }
    // Set flags
    m_registers->setF_Z(m_registers->getA() == 0);
    m_registers->setF_N(1);

    if (n != n8)
        m_registers->addPC(1);
    else m_registers->addPC(2);
}

// 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xDE
void Instructions::SBC_A_n(regID n)
{
    WORD value;
    BYTE result;
    int sum;
    int length = 1;

    switch (n)
    {
    case mHL:
        value = m_memory->read(m_registers->getHL());
        sum = value + m_registers->getF_C();
        break;
    case n8:
        value = read8();
        sum = value + m_registers->getF_C();
        length = 2;
        break;
    default:
        value = m_registers->getReg(n);
        sum = value + m_registers->getF_C();
    }
    result = m_registers->getA() - sum;

    m_registers->setF_Z(!result);
    m_registers->setF_N(1);

    if ((m_registers->getA() & 0x0F) < (value & 0x0F))
        m_registers->setF_H(true);
    else if ((m_registers->getA() & 0x0F) < (sum & 0x0F))
        m_registers->setF_H(true);
    else if (((m_registers->getA() & 0x0F) == (value & 0x0F)) && ((value & 0x0F) == 0x0F) && (m_registers->getF_C()))
        m_registers->setF_H(true);
    else
        m_registers->setF_H(false);

    m_registers->setF_C(m_registers->getA() < sum);

    m_registers->setA(result);

    m_registers->addPC(length);
}

// 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xE6
void Instructions::AND_n(regID n)
{
    int longit = 1;
    switch (n)
    {
    case mHL:
        m_registers->setA(m_memory->read(m_registers->getHL()) & m_registers->getA());
        break;
    case n8:
        m_registers->setA(read8() & m_registers->getA());
        longit = 2;
        break;
    default:
        m_registers->setA(m_registers->getReg(n) & m_registers->getA());
        break;
    }
    // Set Flags
    m_registers->setF_N(0);
    m_registers->setF_C(0);
    m_registers->setF_H(1);
    m_registers->setF_Z(m_registers->getA() == 0);

    m_registers->addPC(longit);
}

// 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xEE
void Instructions::XOR_n(regID n)
{
    switch (n)
    {
    case mHL:
        m_registers->setA(m_memory->read(m_registers->getHL()) ^ m_registers->getA());
        break;
    case n8:
        m_registers->setA(read8() ^ m_registers->getA());
        break;
    default:
        m_registers->setA(m_registers->getReg(n) ^ m_registers->getA());
        break;
    }
    // Set Flags
    m_registers->setF_N(0);
    m_registers->setF_C(0);
    m_registers->setF_H(0);
    m_registers->setF_Z(m_registers->getA() == 0);

    if (n != n8)
        m_registers->addPC(1);
    else m_registers->addPC(2);
}

// 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xF6
void Instructions::OR_n(regID n)
{
    switch (n)
    {
    case mHL:
        m_registers->setA(m_memory->read(m_registers->getHL()) | m_registers->getA());
        break;
    case n8:
        m_registers->setA(read8() | m_registers->getA());
        break;
    default:
        m_registers->setA(m_registers->getReg(n) | m_registers->getA());
        break;
    }

    // Set Flags
    m_registers->setF_N(0);
    m_registers->setF_C(0);
    m_registers->setF_H(0);
    m_registers->setF_Z(m_registers->getA() == 0);

    if (n != n8)
        m_registers->addPC(1);
    else m_registers->addPC(2);
}

// 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xFE
void Instructions::CP_n(regID n)
{
    switch (n)
    {
    case mHL:
        m_registers->setF_H(hasHalfBorrow8(m_registers->getA(), m_memory->read(m_registers->getHL())));
        m_registers->setF_C(hasBorrow8(m_registers->getA(), m_memory->read(m_registers->getHL())));
        m_registers->setF_Z((m_registers->getA() - m_memory->read(m_registers->getHL())) == 0);
        break;
    case n8:
        m_registers->setF_H(hasHalfBorrow8(m_registers->getA(), read8()));
        m_registers->setF_C(hasBorrow8(m_registers->getA(), read8()));
        m_registers->setF_Z((m_registers->getA() - read8()) == 0);
        break;
    default:
        m_registers->setF_H(hasHalfBorrow8(m_registers->getA(), m_registers->getReg(n)));
        m_registers->setF_C(hasBorrow8(m_registers->getA(), m_registers->getReg(n)));
        m_registers->setF_Z((m_registers->getA() - m_registers->getReg(n)) == 0);
        break;
    }
    // Set flags
    m_registers->setF_N(1);

    if (n != n8)
        m_registers->addPC(1);
    else m_registers->addPC(2);
}

// 0xC0, 0xC8, 0xD0, 0xD8
void Instructions::RET_cc(regID cc, bool &condTaken)
{
    switch (cc)
    {
    case nZ:
        if (m_registers->getF_Z() == 0)
        {
            RET();
            condTaken = true;
        }
        else
        {
            m_registers->addPC(1);
            condTaken = false;
        }
        break;
    case Z:
        if (m_registers->getF_Z() == 1)
        {
            RET();
            condTaken = true;
        }
        else
        {
            m_registers->addPC(1);
            condTaken = false;
        }
        break;
    case nC:
        if (m_registers->getF_C() == 0)
        {
            RET();
            condTaken = true;
        }
        else 
        {
            m_registers->addPC(1);
            condTaken = false;
        }
        break;
    case sC:
        if (m_registers->getF_C() == 1)
        {
            RET();
            condTaken = true;
        }
        else 
        {
            m_registers->addPC(1);
            condTaken = false;
        }
        break;
    default:
        cout << "Wrong register identifier set" << endl;
        break;
    }
}

// 0xC1, 0xD1, 0xE1, 0xF1
void Instructions::POP_nn(regID nn)
{
    m_registers->setReg(nn, (m_memory->read(m_registers->getSP() + 1) << 8) | m_memory->read(m_registers->getSP()));
    m_registers->addSP(2);

    m_registers->addPC(1);
}

// 0xC3
void Instructions::JP_nn()
{
    m_registers->setPC(read16());
}

// 0xC2, 0xCA, 0xD2, 0xDA
void Instructions::JP_cc_nn(regID cc, bool &condTaken)
{
    switch (cc)
    {
    case nZ:
        if (m_registers->getF_Z() == 0)
        {
            m_registers->setPC(read16());
            condTaken = true;
        }
        else
        {
            m_registers->addPC(3);
            condTaken = false;
        }
        break;
    case Z:
        if (m_registers->getF_Z() == 1)
        {
            m_registers->setPC(read16());
            condTaken = true;
        }
        else
        {
            m_registers->addPC(3);
            condTaken = false;
        }
        break;
    case nC:
        if (m_registers->getF_C() == 0)
        {
            m_registers->setPC(read16());
            condTaken = true;
        }
        else
        {
            m_registers->addPC(3);
            condTaken = false;
        }
        break;
    case sC:
        if (m_registers->getF_C() == 1)
        {
            m_registers->setPC(read16());
            condTaken = true;
        }
        else
        {
            m_registers->addPC(3);
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
    m_registers->setPC((m_memory->read(m_registers->getSP() + 1) << 8) | m_memory->read(m_registers->getSP()));
    m_registers->addSP(2);
}

// 0xC4, 0xCC, 0xD4, 0xDC
void Instructions::CALL_cc_nn(regID cc, bool &condTaken)
{
    switch (cc)
    {
    case nZ:
        if (m_registers->getF_Z() == 0)
        {
            CALL_nn();
            condTaken = true;
        }
        else
        {
            m_registers->addPC(3);
            condTaken = false;
        }
        break;
    case Z:
        if (m_registers->getF_Z() == 1)
        {
            CALL_nn();
            condTaken = true;
        }
        else
        {
            m_registers->addPC(3);
            condTaken = false;
        }
        break;
    case nC:
        if (m_registers->getF_C() == 0)
        {
            CALL_nn();
            condTaken = true;
        }
        else
        {
            m_registers->addPC(3);
            condTaken = false;
        }
        break;
    case sC:
        if (m_registers->getF_C() == 1)
        {
            CALL_nn();
            condTaken = true;
        }
        else
        {
            m_registers->addPC(3);
            condTaken = false;
        }
        break;
    default:
        cout << "Wrong register identifier set" << endl;
        break;
    }
}

// 0xC5, 0xD5, 0xE5, 0xF5
void Instructions::PUSH_nn(regID nn)
{
    m_registers->addSP(-1);
    m_memory->write(m_registers->getSP(), (m_registers->getReg(nn) & 0xFF00) >> 8);
    m_registers->addSP(-1);
    m_memory->write(m_registers->getSP(), m_registers->getReg(nn) & 0x00FF);
    m_registers->addPC(1);
}

// 0xC7, 0xCF, 0xD7, 0xDF, 0xE7, 0xEF, 0xF7, 0xFF
void Instructions::RST_n(BYTE n)
{
    m_registers->addPC(1);
    m_registers->addSP(-1);
    m_memory->write(m_registers->getSP(), (m_registers->getPC() & 0xFF00) >> 8);
    m_registers->addSP(-1);
    m_memory->write(m_registers->getSP(), m_registers->getPC() & 0x00FF);
    m_registers->setPC(0x0000 + n);
}

// 0xCD
void Instructions::CALL_nn()
{
    m_memory->write(m_registers->getSP() - 1, ((m_registers->getPC() + 3) & 0xFF00) >> 8);
    m_memory->write(m_registers->getSP() - 2, (m_registers->getPC() + 3) & 0x00FF);
    m_registers->setSP(m_registers->getSP() - 2);
    m_registers->setPC(read16());
}

void Instructions::RETI(bool &IME)
{
    m_registers->setPC((m_memory->read(m_registers->getSP() + 1) << 8) | m_memory->read(m_registers->getSP()));
    m_registers->addSP(2);
    IME = true;
}

// 0xE0
void Instructions::LDH_n_A()
{
    m_memory->write(0xFF00 + read8(), m_registers->getA());
    m_registers->addPC(2);
}

// 0xE2
void Instructions::LD_C_A()
{
    m_memory->write(0xFF00 + m_registers->getC(), m_registers->getA());
    m_registers->addPC(1);
}

// 0xE8
void Instructions::ADD_SP_n()
{
    char n = read8();

    m_registers->setF_Z(0);
    m_registers->setF_N(0);

    m_registers->setF_H(((m_registers->getSP() & 0x0F) + (n & 0x0F)) > 0x0F);
    m_registers->setF_C(((m_registers->getSP() & 0xFF) + (n & 0xFF)) > 0xFF);

    m_registers->addSP(n);

    m_registers->addPC(2);
}

// 0xE9
void Instructions::JP_HL()
{
    m_registers->setPC(m_registers->getHL());
}

// 0xF0
void Instructions::LDH_A_n()
{
    m_registers->setA(m_memory->read(0xFF00 + read8()));
    m_registers->addPC(2);
}

// 0xF2
void Instructions::LD_A_C()
{
    m_registers->setA(m_memory->read(0xFF00 + m_registers->getC()));
    m_registers->addPC(1);
}

// 0xF3
void Instructions::DI(bool &IME)
{
    IME = false;
    m_registers->addPC(1);
}

// 0xFB
void Instructions::EI(bool &IME)
{
    IME = true;
    m_registers->addPC(1);
}

// 0xF8
void Instructions::LDHL_SP_n()
{
    // Signed value
    char n = read8();

    m_registers->setF_Z(0);
    m_registers->setF_N(0);
    m_registers->setF_H(((m_registers->getSP() & 0x0F) + (n & 0x0F)) > 0x0F);
    m_registers->setF_C(((m_registers->getSP() & 0xFF) + (n & 0xFF)) > 0xFF);

    m_registers->setHL(m_registers->getSP() + n);

    m_registers->addPC(2);
}

// 0xF9
void Instructions::LD_SP_HL()
{
    m_registers->setSP(m_registers->getHL());
    m_registers->addPC(1);
}


void Instructions::RLC_n(regID n)
{
    BYTE bit7, value;

    if (n == mHL)
    {
        value = m_memory->read(m_registers->getHL());
        bit7 = (value & 0x80) >> 7;
        value = (value << 1) | bit7;
        m_memory->write(m_registers->getHL(), value);
    }
    else
    {
        value = (BYTE)m_registers->getReg(n);
        bit7 = (value & 0x80) >> 7;
        value = (value << 1) | bit7;
        m_registers->setReg(n, value);
    }

    m_registers->setF_Z(value ? 0 : 1);
    m_registers->setF_N(0);
    m_registers->setF_H(0);
    m_registers->setF_C(bit7);

    m_registers->addPC(1);
}


void Instructions::RRC_n(regID n)
{
    BYTE bit0, value;

    if (n == mHL)
    {
        value = m_memory->read(m_registers->getHL());
        bit0 = value & 0x01;
        value = (value >> 1) | (bit0 << 7);
        m_memory->write(m_registers->getHL(), value);
    }
    else
    {
        value = (BYTE)m_registers->getReg(n);
        bit0 = value & 0x01;
        value = (value >> 1) | (bit0 << 7);
        m_registers->setReg(n, value);
    }

    m_registers->setF_Z(value ? 0 : 1);
    m_registers->setF_N(0);
    m_registers->setF_H(0);
    m_registers->setF_C(bit0);

    m_registers->addPC(1);
}

void Instructions::RL_n(regID n)
{
    BYTE oldBit7, value;

    if (n == mHL)
    {
        oldBit7 = (m_memory->read(m_registers->getHL()) & 0x80) >> 7;
        value = (m_memory->read(m_registers->getHL()) << 1) | m_registers->getF_C();
        m_memory->write(m_registers->getHL(), value);
    }
    else
    {
        oldBit7 = (m_registers->getReg(n) & 0x80) >> 7;
        value = (m_registers->getReg(n) << 1) | m_registers->getF_C();
        m_registers->setReg(n, value);
    }

    m_registers->setF_Z(value ? 0 : 1);
    m_registers->setF_N(0);
    m_registers->setF_H(0);
    m_registers->setF_C(oldBit7);


    m_registers->addPC(1);
}

void Instructions::RR_n(regID n)
{
    // Save previous carry
    BYTE prevC = (m_registers->getF() & 0x10) >> 4;
    // Set bit 0 as carry
    if (n != mHL)
        m_registers->setF_C(m_registers->getReg(n) & 0x01);
    else m_registers->setF_C(m_memory->read(m_registers->getHL()) & 0x01);
    // Rotate
    if (n != mHL)
        m_registers->setReg(n, (m_registers->getReg(n) >> 1) | (prevC << 7));
    else m_memory->write(m_registers->getHL(), (m_memory->read(m_registers->getHL()) >> 1) | (prevC << 7));
    // Set flag values
    m_registers->setF_N(0);
    m_registers->setF_H(0);
    if (n != mHL)
        m_registers->setF_Z(m_registers->getReg(n) == 0);
    else m_registers->setF_Z(m_memory->read(m_registers->getHL()) == 0);

    m_registers->addPC(1);
}

void Instructions::SLA_n(regID n)
{
    // Set bit 7 as carry
    if (n != mHL)
        m_registers->setF_C((m_registers->getReg(n) & 0x80) >> 7);
    else m_registers->setF_C((m_memory->read(m_registers->getHL()) & 0x80) >> 7);
    // Left shift
    if (n != mHL) 
        m_registers->setReg(n, ((m_registers->getReg(n) << 1) & 0xFE));
    else m_memory->write(m_registers->getHL(), ((m_memory->read(m_registers->getHL()) << 1) & 0xFE));
    // Set flag values
    m_registers->setF_N(0);
    m_registers->setF_H(0);
    if (n != mHL)
        m_registers->setF_Z(m_registers->getReg(n) == 0);
    else m_registers->setF_Z(m_memory->read(m_registers->getHL()) == 0);

    m_registers->addPC(1);
}

void Instructions::SRA_n(regID n)
{
    BYTE bit0, bit7, value;

    if (n == mHL)
    {
        bit0 = (m_memory->read(m_registers->getHL()) & 0x01);
        bit7 = (m_memory->read(m_registers->getHL()) & 0x80);
        m_memory->write(m_registers->getHL(), bit7 | (m_memory->read(m_registers->getHL()) >> 1));
        value = m_memory->read(m_registers->getHL());
    }
    else
    {
        bit0 = (m_registers->getReg(n) & 0x01);
        bit7 = (m_registers->getReg(n) & 0x80);
        m_registers->setReg(n, bit7 | (m_registers->getReg(n) >> 1));
        value = (BYTE)m_registers->getReg(n);
    }

    m_registers->setF_Z(!value ? 1 : 0);
    m_registers->setF_N(0);
    m_registers->setF_H(0);
    m_registers->setF_C(bit0);

    m_registers->addPC(1);
}

void Instructions::SRL_n(regID n)
{
    // Set bit 7 as carry
    if (n != mHL)
        m_registers->setF_C(m_registers->getReg(n) & 0x01);
    else m_registers->setF_C(m_memory->read(m_registers->getHL()) & 0x01);
    // Left shift
    if (n != mHL)
        m_registers->setReg(n, (m_registers->getReg(n) >> 1));
    else m_memory->write(m_registers->getHL(), (m_memory->read(m_registers->getHL()) >> 1));
    // Set flag values
    m_registers->setF_N(0);
    m_registers->setF_H(0);
    if (n != mHL)
        m_registers->setF_Z(m_registers->getReg(n) == 0);
    else m_registers->setF_Z(m_memory->read(m_registers->getHL()) == 0);

    m_registers->addPC(1);
}

// CB 0x37
void Instructions::SWAP_n(regID n)
{
    BYTE value;

    if (n == mHL)
    {
        value = m_memory->read(m_registers->getHL());
        value = ((value & 0x0F) << 4) | ((value & 0xF0) >> 4);
        m_memory->write(m_registers->getHL(), value);
    }
    else
    {
        value = (BYTE)m_registers->getReg(n);
        value = ((value & 0x0F) << 4) | ((value & 0xF0) >> 4);
        m_registers->setReg(n, value);
    }

    m_registers->setF_Z(value ? 0 : 1);
    m_registers->setF_N(0);
    m_registers->setF_H(0);
    m_registers->setF_C(0);

    m_registers->addPC(1);
}

// CB 0x7F
void Instructions::BIT_b_r(BYTE b, regID r)
{
    if (r != mHL)
        m_registers->setF_Z((m_registers->getReg(r) & (0x01 << b)) == 0);
    else m_registers->setF_Z((m_memory->read(m_registers->getHL()) & (0x01 << b)) == 0);
    m_registers->setF_N(0);
    m_registers->setF_H(1);

    m_registers->addPC(1);
}

void Instructions::SET_b_r(BYTE b, regID r)
{
    if (r != mHL)
        m_registers->setReg(r, m_registers->getReg(r) | (0x01 << b));
    else m_memory->write(m_registers->getHL(), m_memory->read(m_registers->getHL()) | (0x01 << b));

    m_registers->addPC(1);
}

void Instructions::RES_b_r(BYTE b, regID r)
{
    if (r != mHL)
        m_registers->setReg(r, m_registers->getReg(r) & ~(0x01 << b));
    else m_memory->write(m_registers->getHL(), m_memory->read(m_registers->getHL()) & ~(0x01 << b));

    m_registers->addPC(1);
}
