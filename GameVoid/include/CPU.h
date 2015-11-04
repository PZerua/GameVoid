#pragma once

#include "Header.h"
#include "Memory.h"
#include "Registers.h"
#include "Instructions.h"

class CPU
{
public:
	CPU(Memory *memory);
	~CPU();
	void run();

private:
	Registers _registers;
	Memory *_memory;
	Instructions *_inst;
};
