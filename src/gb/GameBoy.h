#pragma once

#include "header.h"
#include "cartridge.h"
#include "memory.h"
#include "cpu.h"
#include "video.h"
#include "controller.h"
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
    Cartridge m_game;
    Memory m_memory;
    Video m_video;
    CPU m_cpu;
    Controller m_controller;
    gfx::Window m_window;
};
