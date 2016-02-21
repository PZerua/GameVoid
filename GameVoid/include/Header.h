#pragma once

#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

using namespace std;

// Window sizes
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144

// Clock frequency
#define CLOCKSPEED 4194304

// GameBoy color palette
#define COLOR_GREEN_0 0x9cbd0f
#define COLOR_GREEN_1 0x8cad0f
#define COLOR_GREEN_2 0x306230
#define COLOR_GREEN_3 0x0f380f

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
#define LY		0xFF44
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

// In C++ char has 1 Byte width
// It has to be unsigned because we need up to 255 values
// In signed types 1 bit is used to denote sign
typedef unsigned char BYTE;

// Short has 2 Bytes width
typedef unsigned short WORD;

typedef signed char SIGNED_BYTE;

typedef signed short SIGNED_WORD;

enum Interrupt {
	VBlank,
	LCD,
	Timer,
	CLink,
	JoyPad
};

enum COLOUR {
	WHITE,
	LIGHT_GREY,
	DARK_GREY,
	BLACK
};

BYTE bitSet(const BYTE &byte, const int &bit);
BYTE bitReset(const BYTE &byte, const int &bit);
bool testBit(const BYTE &byte, const int &bit);
BYTE bitGetVal(const BYTE &byte, const int &bit);

static WORD lastPC;
static BYTE lastOPCODE;