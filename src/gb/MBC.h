#pragma once
#include "Header.h"

class MBC
{
public:
    virtual BYTE read(WORD address) = 0;
    virtual void write(WORD adress, BYTE value) = 0;

protected:
    unsigned _RAMsize;
    bool _RAMenabled;
    bool _mode;
    unsigned _ROMbank;
    unsigned _RAMbank;

    enum {
        ROMmode,
        RAMmode
    };
};
