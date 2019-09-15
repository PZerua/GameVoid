#include "GameBoy.h"

int main(int argc, char* argv[])
{
    GameBoy *_gameboy = new GameBoy();
    if (!_gameboy->init()) {
        system("pause");
        return 1;
    }

    _gameboy->update();
    
    system("pause");

    return 0;
}
