#pragma once

#include "data_types.h"

class Controller
{
public:
    Controller() = default;
    void init(BYTE *mem);
    BYTE getJoypadState() const;
    void checkControls();
    void keyPressed(int key);
    bool interruptRequested();
    void setInterruptRequested(bool state);
private:
    BYTE  m_joypadState;
    BYTE* m_memory;
    bool  m_interruptRequested;
};