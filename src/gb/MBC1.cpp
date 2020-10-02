#include "mbc1.h"

MBC1::MBC1(BYTE *ROMdata, unsigned RAMsize)
{
    m_ROMdata = ROMdata;
    memset(m_RAMdata, 0, RAMsize);
    m_RAMsize = RAMsize;
    m_RAMenabled = false;
    m_mode = 0;
    m_ROMbank = 1;
    m_RAMbank = 0;
}

MBC1::~MBC1()
{

}

BYTE MBC1::read(WORD address)
{
    // Read ROM Bank 0
    if (address >= 0x0000 && address < 0x4000)
    {
        return m_ROMdata[address];
    }
    // Read ROM Bank 0x01 to 0x7f (up to 255)
    else if (address >= 0x4000 && address < 0x8000)
    {
        /*if (_ROMbank == 0x00 || _ROMbank == 0x20 || 
            _ROMbank == 0x40 || _ROMbank == 0x60)
            _ROMbank++;*/ // TODO: investigate

        return m_ROMdata[(0x4000 * m_ROMbank) + (address - 0x4000)];
    }
    // Read RAM Bank 0x00 to 0x03 
    else if (address >= 0xA000 && address < 0xC000)
    {
        if (m_RAMsize == 0 || !m_RAMenabled)
        {
            cout << hex << "PC: " << (int)lastPC << " | OPCODE: " << (int)lastOPCODE << endl;
            throw exception("Trying to read uninitialized or null RAM");
        }    
        return m_RAMdata[(0x2000 * m_RAMbank) + (address - 0xA000)];
    }
    else
    {
        cout << hex << "PC: " << (int)lastPC << " | OPCODE: " << (int)lastOPCODE << endl;
        throw exception("Wrong address");
    }
}

void MBC1::write(WORD address, BYTE value)
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
    // Select ROM Bank Number (only lower 5 bits)
    else if (address >= 0x2000 && address < 0x4000)
    {
        BYTE tempValue = value;
        if ((value & 0x1F) == 0)
            tempValue++;
        m_ROMbank = (tempValue & 0x1F) | (m_ROMbank & 0x60);
    }
    // Select RAM Bank Number or upper bits of ROM Bank Number
    else if (address >= 0x4000 && address < 0x6000)
    {
        if (m_mode == ROMmode)
            m_ROMbank = (m_ROMbank & 0x1F) | ((value & 0x03) << 5);
        else if (m_mode == RAMmode)
            m_RAMbank = value & 0x03;
    }
    // Select ROM or RAM mode
    else if (address >= 0x6000 && address < 0x8000)
    {
        if (value == 0x00)
            m_mode = ROMmode;
        else if (value == 0x01)
            m_mode = RAMmode;
    }
    // Write RAM Bank 0x00 to 0x03 
    else if (address >= 0xA000 && address < 0xC000)
    {
        if (m_RAMsize != 0 && m_RAMenabled)
        {
            m_RAMdata[(0x2000 * m_RAMbank) + (address - 0xA000)] = value;
        }
    }
}
