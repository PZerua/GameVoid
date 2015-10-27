#pragma once
#include "Header.h"

#define HEADER_TITLE 0x134
#define HEADER_CGB_FLAG 0x143
#define HEADER_SGB_FLAG 0x146
#define HEADER_CART_TYPE 0x147
#define HEADER_ROM_SIZE 0x148
#define HEADER_RAM_SIZE 0x149
#define HEADER_DEST_CODE 0x14A

class Cartridge
{
public:
	Cartridge(const string &gamePath);
	~Cartridge();
	void init();

private:
	BYTE *ROMdata;
	int _ROMsize;
	unsigned _RAMsize;
	bool _CGBfeatures;
	bool _SGBfeatures;
	char _title[17];
};
