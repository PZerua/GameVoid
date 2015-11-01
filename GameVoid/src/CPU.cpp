#include "CPU.h"
 
CPU::CPU(Memory *memory)
{
	_memory = memory;
}

CPU::~CPU()
{

}

void CPU::run()
{
	_instructions = new Instructions(_memory, &_registers);
	bool run = true;

	while (run)
	{
		WORD OPCODE = _memory->read(_registers.getPC());

		switch (OPCODE)
		{
		case 0x0000:
			_instructions->NOP();
			break;
		default:
			cout << "Unknown or not implemented instruction: " << hex << OPCODE << dec << endl;
			run = false;
			break;
		}
	}

}
