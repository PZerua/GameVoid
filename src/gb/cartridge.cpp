#include "cartridge.h"

#include <iostream>
#include <fstream>

#include <string>

#include "mbc/none.h"
#include "mbc/mbc1.h"
#include "mbc/mbc3.h"

using namespace std;

Cartridge::~Cartridge()
{
    // Delete array in destructor
    if (m_ROMdata) {
        delete[] m_ROMdata;
    }
}

bool Cartridge::initGame(const string &gamePath)
{
    // With ios::ate, the initial position will be the end of the file
    ifstream rom(gamePath, ios::binary | ios::ate);

    // tellg() returns the actual position, since we are in the end of file, it returns the size
    m_ROMsize = (int)rom.tellg();

    if (m_ROMsize != -1) {
        // Points to the beginning of the ifle
        rom.seekg(0, ios::beg);

        // Array of BYTEs containing the ROM
        m_ROMdata = new BYTE[m_ROMsize];

        rom.read((char *)m_ROMdata, m_ROMsize);

        // Read info from ROM
        loadHeader();
    } else {
        cout << "Error finding path or reading rom: " << gamePath << endl;
        return false;
    }

    rom.close();

    return true;
}

void Cartridge::loadHeader()
{

    // Get game title
    memcpy(m_title, &m_ROMdata[HEADER_TITLE], 17);
    m_title[16] = '\0';
    cout << "GAME TITLE: " << m_title << endl;

    // We check if the size of ROM was properly calculated
    int tempSize = 0;

    // From ROM Header documentation: size = 32KB << N
    tempSize = 32768 << m_ROMdata[HEADER_ROM_SIZE];

    if (tempSize != m_ROMsize) {
        cout << "Error reading ROM size" << endl;
    } else { 
        cout << "GAME SIZE: " << m_ROMsize << " KB" << endl; 
    }

    // Read RAM size info
    switch (m_ROMdata[HEADER_RAM_SIZE])
    {
    case 0x00:
        m_RAMsize = 0; // 0 KB
        break;
    case 0x01:
        m_RAMsize = 2024; // 2 KB
        break;
    case 0x02:
        m_RAMsize = 8192; // 8 KB
        break;
    case 0x03:
        m_RAMsize = 32768; // 32 KB, 4 banks of 8 KB each
        break;
    case 0x04:
        m_RAMsize = 131072; // 128 KB, 16 banks of 8 KB each
        break;
    case 0x05:
        m_RAMsize = 65536; // 64 KB, 8 banks of 8 KB each
        break;
    default:
        cout << "Error reading RAM size" << endl;
        break;
    }
    cout << "RAM SIZE: " << m_RAMsize << " KB" << endl;

    BYTE cartType = m_ROMdata[HEADER_CART_TYPE];
    // Read Cartridge type info
    switch (cartType)
    {
    case 0x00:        // ROM
    case 0x08:        // ROM+RAM
    case 0x09:        // ROM+RAM+BATTERY
        cout << "CARTRIDGE TYPE: " << hex << (int)cartType << " - NO MBC" << dec << endl;
        m_MBC = new None(m_ROMdata, m_RAMsize);
        break;
    case 0x01:        // MBC1
    case 0x02:        // MBC1+RAM
    case 0x03:        // MBC1+RAM+BATTERY
        cout << "CARTRIDGE TYPE: " << hex << (int)cartType << " - MBC1" << dec << endl;
        m_MBC = new MBC1(m_ROMdata, m_RAMsize);
        break;
    case 0x11:        // MBC3
    case 0x12:        // MBC3+RAM
    case 0x13:        // MBC3+RAM+BATTERY
        cout << "CARTRIDGE TYPE: " << hex << (int)cartType << " - MBC3" << dec << endl;
        m_MBC = new MBC3(m_ROMdata, m_RAMsize);
        break;
    case 0x05:        // MBC2
    case 0x06:        // MBC2+BATTERY
    case 0x0B:        // MMM01
    case 0x0C:        // MMM01+RAM
    case 0x0D:        // MMM01+RAM+BATTERY
    case 0x0F:        // MBC3+TIMER+BATTERY
    case 0x10:        // MBC3+TIMER+RAM+BATTERY
    case 0x15:        // MBC4
    case 0x16:        // MBC4+RAM
    case 0x17:        // MBC4+RAM+BATTERY
    case 0x19:        // MBC5
    case 0x1A:        // MBC5+RAM
    case 0x1B:        // MBC5+RAM+BATTERY
    case 0x1C:        // MBC5+RUMBLE
    case 0x1D:        // MBC5+RUMBLE+RAM
    case 0x1E:        // MBC5+RUMBLE+RAM+BATTERY
    case 0xFC:        // POCKET CAMERA
    case 0xFD:        // Bandai TAMA5
    case 0xFE:        // HuC3
    case 0xFF:        // HuC1+RAM+BATTERY
    default:
        cout << "Unknown cartridge type or not implemented: " << hex << (int)cartType << dec << endl;
        break;
    }
}

BYTE *Cartridge::getData()
{
    return m_ROMdata;
}