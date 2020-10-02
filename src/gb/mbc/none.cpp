#include "none.h"

#include <iostream>

using namespace std;

None::None(BYTE *ROMdata, unsigned RAMsize) :
    m_ROMdata(ROMdata),
    MBC(RAMsize)
{}

BYTE None::read(WORD address)
{
    // Read ROM data
    if (address >= 0x0000 && address < 0x8000)
    {
        return m_ROMdata[address];
    }
    // Read RAM data 
    else if (address >= 0xA000 && address < 0xC000)
    {
        if (m_RAMsize == 0 || !m_RAMenabled)
            throw exception("Trying to read uninitialized or null RAM");
        return m_ROMdata[address];
    }
    else throw exception("Wrong address");
}

void None::write(WORD address, BYTE value)
{
    // Enable or disable RAM
    if (address >= 0x0000 && address < 0x2000)
    {
        // Any value with 0x0A in lower 4 bits enables RAM
        if ((value & 0x0F) == 0x0A)
        {
            m_RAMenabled = true;
        }
        // Other value disables it
        else
        {
            m_RAMenabled = false;
        }
    }
    // Write to RAM 
    else if (address >= 0xA000 && address < 0xC000)
    {
        if (m_RAMsize != 0 && m_RAMenabled)
            m_ROMdata[address] = value;
    }
    //else cout << "Error: writing to a ROM without MBC" << endl;
}
