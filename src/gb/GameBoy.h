#pragma once

#include "Header.h"
#include "Cartridge.h"
#include "Memory.h"
#include "CPU.h"
#include "Video.h"
#include "Controller.h"
#include "graphics/window.h"

constexpr auto MAXCYCLES = 69905;

class GameBoy
{
public:
    GameBoy();
    ~GameBoy();
    bool init(const std::string romPath);
    void start();

private:
    Cartridge _game;
    Memory _memory;
    Video _video;
    CPU _CPU;
    Controller _controller;
    gfx::Window _window;
};
