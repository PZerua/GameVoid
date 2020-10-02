#pragma once

#include "mbc.h"

class MBC1 : public MBC
{
public:
    MBC1(BYTE *ROMdata, unsigned RAMsize);
    BYTE read(WORD address);
    void write(WORD address, BYTE value);
private:
    BYTE* m_ROMdata;
    BYTE  m_RAMdata[0x8000] = {};
};
