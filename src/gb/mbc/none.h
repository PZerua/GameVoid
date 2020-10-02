#pragma once

#include "mbc.h"

class None : public MBC
{
public:
    None(BYTE *ROMdata, unsigned RAMsize);
    BYTE read(WORD address);
    void write(WORD address, BYTE value);
private:
    BYTE* m_ROMdata;
};
