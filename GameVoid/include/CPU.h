#pragma once

#include "Header.h"
#include "Memory.h"
#include "Registers.h"
#include "Instructions.h"
#include "Video.h"

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
	void updateTimers(const int &cycles);
	int selectFrequency();
	bool isClockEnabled();
	void divideRegister(const int &cycles);
	void requestInterrupt(const int &id);
	void doInterrupts();
	void serviceInterrupt(const int &id);

private:
	Registers _registers;
	Memory *_memory;
	Instructions *_inst;

	short _instCycles[0x100];
	short _CBinstCycles[0x100];
	bool IME;
	bool _condTaken;
	int _timeCounter;
	int _divideCounter;
	int _timeFrequency;
};
