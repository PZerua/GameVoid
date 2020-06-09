#include "Cartridge.h"

#include <string>

#include "MBC1.h"
#include "MBC3.h"

Cartridge::Cartridge()
{
    _ROMsize = 0;
    _RAMsize = 0;
}

bool Cartridge::initGame(const string &gamePath)
{
    // With ios::ate, the initial position will be the end of the file
    ifstream rom(gamePath, ios::binary | ios::ate);

    // tellg() returns the actual position, since we are in the end of file, it returns the size
    _ROMsize = (int)rom.tellg();

    if (_ROMsize != -1)
    {
        // Points to the beginning of the ifle
        rom.seekg(0, ios::beg);

        // Array of BYTEs containing the ROM
        _ROMdata = new BYTE[_ROMsize];

        rom.read((char *)_ROMdata, _ROMsize);

        // Read info from ROM
        loadHeader();
    }
    else
    {
        cout << "Error finding path or reading rom: " << gamePath << endl;
        return false;
    }

    rom.close();

    return true;
}


Cartridge::~Cartridge()
{
    // Delete array in destructor
    if (_ROMdata) {
        delete[] _ROMdata;
    }
}

void Cartridge::loadHeader()
{

    // Get game title
    memcpy(_title, &_ROMdata[HEADER_TITLE], 17);
    _title[16] = '\0';
    cout << "GAME TITLE: " << _title << endl;

    // We check if the size of ROM was properly calculated
    int tempSize = 0;

    // From ROM Header documentation: size = 32KB << N
    tempSize = 32768 << _ROMdata[HEADER_ROM_SIZE];

    if (tempSize != _ROMsize)
        cout << "Error reading ROM size" << endl;
    else cout << "GAME SIZE: " << _ROMsize << " KB" << endl;

    // Read RAM size info
    switch (_ROMdata[HEADER_RAM_SIZE])
    {
    case 0x00:
        _RAMsize = 0; // 0 KB
        break;
    case 0x01:
        _RAMsize = 2024; // 2 KB
        break;
    case 0x02:
        _RAMsize = 8192; // 8 KB
        break;
    case 0x03:
        _RAMsize = 32768; // 32 KB, 4 banks of 8 KB each
        break;
    case 0x04:
        _RAMsize = 131072; // 128 KB, 16 banks of 8 KB each
        break;
    case 0x05:
        _RAMsize = 65536; // 64 KB, 8 banks of 8 KB each
        break;
    default:
        cout << "Error reading RAM size" << endl;
        break;
    }
    cout << "RAM SIZE: " << _RAMsize << " KB" << endl;

    BYTE cartType = _ROMdata[HEADER_CART_TYPE];
    // Read Cartridge type info
    switch (cartType)
    {
    case 0x00:        // ROM
    case 0x08:        // ROM+RAM
    case 0x09:        // ROM+RAM+BATTERY
        cout << "CARTRIDGE TYPE: " << hex << (int)cartType << " - NO MBC" << dec << endl;
        _MBC = new None(_ROMdata, _RAMsize);
        break;
    case 0x01:        // MBC1
    case 0x02:        // MBC1+RAM
    case 0x03:        // MBC1+RAM+BATTERY
        cout << "CARTRIDGE TYPE: " << hex << (int)cartType << " - MBC1" << dec << endl;
        _MBC = new MBC1(_ROMdata, _RAMsize);
        break;
    case 0x11:        // MBC3
    case 0x12:        // MBC3+RAM
    case 0x13:        // MBC3+RAM+BATTERY
        cout << "CARTRIDGE TYPE: " << hex << (int)cartType << " - MBC3" << dec << endl;
        _MBC = new MBC3(_ROMdata, _RAMsize);
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
    return _ROMdata;
}