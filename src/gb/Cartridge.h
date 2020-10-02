#pragma once

#include <string>

#include "data_types.h"
#include "mbc/mbc.h"
#include "mbc/none.h"

#define HEADER_TITLE        0x134
#define HEADER_CGB_FLAG     0x143
#define HEADER_SGB_FLAG     0x146
#define HEADER_CART_TYPE    0x147
#define HEADER_ROM_SIZE     0x148
#define HEADER_RAM_SIZE     0x149
#define HEADER_DEST_CODE    0x14A

class Cartridge
{
public:
    Cartridge() = default;
    ~Cartridge();
    void  loadHeader();
    bool  initGame(const std::string &gamePath);
    BYTE* getData();
    MBC*  _MBC;

private:
    BYTE*       m_ROMdata = nullptr;
    int         m_ROMsize = 0;
    unsigned    m_RAMsize = 0;
    bool        m_CGBfeatures = false;
    bool        m_SGBfeatures = false;
    char        m_title[17] = {};
};
