#pragma once

#include "data_types.h"

class Controller
{
public:
    Controller() = default;
    void init(BYTE *memory);
    BYTE getJoypadState() const;
    void checkControls();
    void keyPressed(int key);
    bool interruptRequested();
    void setInterruptRequested(bool state);
private:
    BYTE  m_joypadState = 0xFF;
    BYTE* m_memory = nullptr;
    bool  m_interruptRequested = false;
};