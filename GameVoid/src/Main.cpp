#include "Header.h"
#include "Cartridge.h"
#include "Memory.h"
#include "CPU.h"

int main(int argc, char* argv[])
{
	Cartridge game("rom/SMLand.gb");

	Memory memory(&game);

	CPU cpu(&memory);

	cpu.run();

	system("pause");

}
