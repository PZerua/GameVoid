#pragma once

#include "gb/data_types.h"

class MBC
{
public:
    MBC(unsigned RAMSize) : m_RAMsize(RAMSize) {}
    virtual BYTE read(WORD address) = 0;
    virtual void write(WORD adress, BYTE value) = 0;

protected:
    unsigned    m_RAMsize = 0;
    bool        m_RAMenabled = false;
    bool        m_mode = 0;
    unsigned    m_ROMbank = 1;
    unsigned    m_RAMbank = 0;

    enum {
        ROMmode,
        RAMmode
    };
};
