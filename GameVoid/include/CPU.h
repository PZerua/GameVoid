#pragma once

#include "Header.h"
#include "Memory.h"
#include "Registers.h"
#include "Instructions.h"

class CPU
{
public:
	CPU() {}
	~CPU() {}
	void init(Memory *memory);
	int fetch();
	int CB_prefix(Instructions *inst);
	void initCyclesArrays();
	void condCycles();
	void uncondCycles();
	void requestInterrupt(int id);
	void doInterrupts();
	void serviceInterrupt(int id);
	BYTE getClockFreq() const;
	void setClockFreq();
	void updateTimers(int cycles);
	bool isClockEnabled();
	void divideRegister(int cycles);
	int _divideCounter;

private:
	Registers _registers;
	Memory *_memory;
	Instructions *_inst;

	short _instCycles[0x100];
	short _CBinstCycles[0x100];
	bool IME;
	bool _condTaken;
	int _timeCounter;
	
};
