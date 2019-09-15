#pragma once
#include "Header.h"

class Controller
{
public:
    Controller();
    ~Controller();
    void init(BYTE *mem);
    BYTE getJoypadState() const;
    void checkControls();
    void keyPressed(int key);
    bool requestInterrupt();
    void setRequestInterrupt(bool state);
private:
    BYTE _joypadState;
    BYTE *memory;
    bool _requestInterrupt;
};