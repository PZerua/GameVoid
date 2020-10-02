#pragma once
#include "header.h"

class MBC
{
public:
    virtual BYTE read(WORD address) = 0;
    virtual void write(WORD adress, BYTE value) = 0;

protected:
    unsigned    m_RAMsize;
    bool        m_RAMenabled;
    bool        m_mode;
    unsigned    m_ROMbank;
    unsigned    m_RAMbank;

    enum {
        ROMmode,
        RAMmode
    };
};
