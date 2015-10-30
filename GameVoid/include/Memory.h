#pragma once
#include "Header.h"
#include "Cartridge.h"

#define MEM_SIZE 65536

// Timer and Divider Registers
#define TIMA	0xFF05
#define TMA		0xFF06
#define TAC		0xFF07
// Sound Registers
#define NR10	0xFF10
#define NR11	0xFF11
#define NR12	0xFF12
#define NR14	0xFF14
#define NR21	0xFF16
#define NR22	0xFF17
#define NR24	0xFF19
#define NR30	0xFF1A
#define NR31	0xFF1B
#define NR32	0xFF1C
#define NR33	0xFF1E
#define NR41	0xFF20
#define NR42	0xFF21
#define NR43	0xFF22
#define NR44	0xFF23
#define NR50	0xFF24
#define NR51	0xFF25
#define NR52	0xFF26
// LCD Control Registers
#define LCDC	0xFF40
#define SCY		0xFF42
#define SCX		0xFF43
#define LYC		0xFF45
// LCD Monochrome Palettes
#define BGP		0xFF47
#define OBP0	0xFF48
#define OBP1	0xFF49
// LCD Position and Scrolling
#define WY		0xFF4A
#define WX		0xFF4B
// Interrupt Register
#define IE		0xFFFF

class Memory
{
public:
	Memory(Cartridge *cartridge);
	~Memory();
	void reset();

private:
	BYTE _MEMORY[MEM_SIZE];
	Cartridge *_cartridge;
	BYTE read(const WORD &address);
	void write(const WORD &address, const BYTE &value);
};