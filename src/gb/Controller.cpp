#include "Controller.h"

#include "bitUtils.h"
#include "input.h"

Controller::Controller()
{

}

Controller::~Controller()
{

}

void Controller::init(BYTE *mem)
{
    memory = mem;
    bool keydown = false;
    bool keyup = false;
    m_interruptRequested = false;
    _joypadState = 0xFF;
}

BYTE Controller::getJoypadState() const
{
    BYTE res = memory[0xFF00];
    // flip all the bits
    res ^= 0xFF;

    // are we interested in the standard buttons?
    if (!testBit(res, 4))
    {
        BYTE topJoypad = _joypadState >> 4;
        topJoypad |= 0xF0; // turn the top 4 bits on
        res &= topJoypad; // show what buttons are pressed
    }
    else if (!testBit(res, 5))//directional buttons
    {
        BYTE bottomJoypad = _joypadState & 0xF;
        bottomJoypad |= 0xF0;
        res &= bottomJoypad;
    }
    return res;
}

void Controller::keyPressed(int key)
{
    bool previouslyUnset = false;

    // if setting from 1 to 0 we may have to request an interupt
    if (testBit(_joypadState, key) == false)
        previouslyUnset = true;

    // remember if a keypressed its bit is 0 not 1
    _joypadState = bitReset(_joypadState, key);

    // button pressed
    bool button = true;

    // is this a standard button or a directional button?
    if (key > 3)
        button = true;
    else // directional button pressed
        button = false;

    BYTE keyReq = memory[0xFF00];
    bool requestInterupt = false;

    // only request interupt if the button just pressed is
    // the style of button the game is interested in
    if (button && !testBit(keyReq, 5))
        requestInterupt = true;

    // same as above but for directional button
    else if (!button && !testBit(keyReq, 4))
        requestInterupt = true;

    // request interupt
    if (requestInterupt && !previouslyUnset)
        m_interruptRequested = true;
}

void Controller::checkControls()
{
    input::Keyboard &input = input::Keyboard::getInstance();

    if (input.isPressed(GLFW_KEY_A)) {
        keyPressed(4);
    }
    else {
        _joypadState = bitSet(_joypadState, 4);
    }

    if (input.isPressed(GLFW_KEY_S)) {
        keyPressed(5);
    }
    else {
        _joypadState = bitSet(_joypadState, 5);
    }

    if (input.isPressed(GLFW_KEY_ENTER)) {
        keyPressed(7);
    }
    else {
        _joypadState = bitSet(_joypadState, 7);
    }

    if (input.isPressed(GLFW_KEY_SPACE)) {
        keyPressed(6);
    }
    else {
        _joypadState = bitSet(_joypadState, 6);
    }

    if (input.isPressed(GLFW_KEY_RIGHT)) {
        keyPressed(0);
    }
    else {
        _joypadState = bitSet(_joypadState, 0);
    }

    if (input.isPressed(GLFW_KEY_LEFT)) {
        keyPressed(1);
    }
    else {
        _joypadState = bitSet(_joypadState, 1);
    }

    if (input.isPressed(GLFW_KEY_UP)) {
        keyPressed(2);
    }
    else {
        _joypadState = bitSet(_joypadState, 2);
    }

    if (input.isPressed(GLFW_KEY_DOWN)) {
        keyPressed(3);
    }
    else {
        _joypadState = bitSet(_joypadState, 3);
    }
}

bool Controller::interruptRequested()
{
    return m_interruptRequested;
}

void Controller::setInterruptRequested(bool state)
{
    m_interruptRequested = state;
}