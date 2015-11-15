#include "GameBoy.h"

GameBoy::GameBoy()
{
	_game.initGame("rom/SMLand.gb"); //For testing
	_memory.init(&_game);
	_CPU.init(&_memory);
}

GameBoy::~GameBoy()
{

}

void GameBoy::init()
{
	_CPU.run();
}
