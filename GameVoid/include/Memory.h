#pragma once
#include "Header.h"
#include "Cartridge.h"

#define MEM_SIZE 65535

class Memory
{
public:
	Memory(Cartridge *cartridge);
	~Memory();

private:
	BYTE _MEMORY[MEM_SIZE];
	Cartridge *_cartridge;
	BYTE read(const WORD &address);
	void write(const WORD &address, const BYTE &value);
};