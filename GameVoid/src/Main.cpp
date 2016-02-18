#include "GameBoy.h"

int main(int argc, char* argv[])
{
	GameBoy *_gameboy = new GameBoy();

	_gameboy->update();
	
	system("pause");

	return 1;
}
