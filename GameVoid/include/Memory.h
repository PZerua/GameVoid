#pragma once
#include "Header.h"
#include "Cartridge.h"

#define MEM_SIZE 65536

class Memory
{
public:
	Memory() {}
	~Memory() {}
	void init(Cartridge *cartridge);
	void reset();
	BYTE read(const WORD &address);
	void write(const WORD &address, const BYTE &value);
	void directModification(const WORD &address, const BYTE &value);

private:
	BYTE _MEMORY[MEM_SIZE];
	Cartridge *_cartridge;
	
};
