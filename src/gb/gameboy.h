#pragma once

#include "cartridge.h"
#include "memory.h"
#include "cpu.h"
#include "video.h"
#include "controller.h"
#include "graphics/window.h"

constexpr auto MAXCYCLES = 70224;
constexpr double MICROSECONDS_PER_CYCLE = 0.2384;

class GameBoy
{
public:
    GameBoy() = default;
    bool init(const std::string romPath);
    void tick(double deltaTime);

private:
    Cartridge cartridge;
    Memory memory;
    Video video;
    CPU cpu;
    Controller controller;
};
