#include "Instructions.h"

Instructions::Instructions(Memory *memory, Registers *registers)
{
	_memory = memory;
	_registers = registers;
}

Instructions::~Instructions()
{

}

// 0x0000
void Instructions::NOP()
{
	_registers->addPC(1);
}