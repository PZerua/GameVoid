#include "gb/GameBoy.h"

#include <iostream>

int main(int argc, char* argv[])
{

    if (argc == 1) {
#ifdef __linux__
        std::cout << "Usage: \nGameVoid <path to rom>" << std::endl;
#elif _WIN32
        std::cout << "Usage: \nGameVoid.exe <path to rom>" << std::endl;
#endif
        system("pause");
        return 0;
    }
    
    char *romPath = argv[1];
    //char *romPath = R"(C:\Users\Pablo\Desktop\rom\test.gb)";

    GameBoy gameboy;
    if (!gameboy.init(romPath)) {
        system("pause");
        return 1;
    }

    gameboy.start();

    return 0;
}
