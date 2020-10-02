#include "controller.h"

#include "utils/bitUtils.h"
#include "input/input.h"

using namespace utils;

Controller::Controller()
{

}

Controller::~Controller()
{

}

void Controller::init(BYTE *mem)
{
    m_memory = mem;
    bool keydown = false;
    bool keyup = false;
    m_interruptRequested = false;
    m_joypadState = 0xFF;
}

BYTE Controller::getJoypadState() const
{
    BYTE res = m_memory[0xFF00];
    // flip all the bits
    res ^= 0xFF;

    // are we interested in the standard buttons?
    if (!testBit(res, 4))
    {
        BYTE topJoypad = m_joypadState >> 4;
        topJoypad |= 0xF0; // turn the top 4 bits on
        res &= topJoypad; // show what buttons are pressed
    }
    else if (!testBit(res, 5))//directional buttons
    {
        BYTE bottomJoypad = m_joypadState & 0xF;
        bottomJoypad |= 0xF0;
        res &= bottomJoypad;
    }
    return res;
}

void Controller::keyPressed(int key)
{
    bool previouslyUnset = false;

    // if setting from 1 to 0 we may have to request an interupt
    if (testBit(m_joypadState, key) == false)
        previouslyUnset = true;

    // remember if a keypressed its bit is 0 not 1
    m_joypadState = bitReset(m_joypadState, key);

    // button pressed
    bool button = true;

    // is this a standard button or a directional button?
    if (key > 3)
        button = true;
    else // directional button pressed
        button = false;

    BYTE keyReq = m_memory[0xFF00];
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
        m_joypadState = bitSet(m_joypadState, 4);
    }

    if (input.isPressed(GLFW_KEY_S)) {
        keyPressed(5);
    }
    else {
        m_joypadState = bitSet(m_joypadState, 5);
    }

    if (input.isPressed(GLFW_KEY_ENTER)) {
        keyPressed(7);
    }
    else {
        m_joypadState = bitSet(m_joypadState, 7);
    }

    if (input.isPressed(GLFW_KEY_SPACE)) {
        keyPressed(6);
    }
    else {
        m_joypadState = bitSet(m_joypadState, 6);
    }

    if (input.isPressed(GLFW_KEY_RIGHT)) {
        keyPressed(0);
    }
    else {
        m_joypadState = bitSet(m_joypadState, 0);
    }

    if (input.isPressed(GLFW_KEY_LEFT)) {
        keyPressed(1);
    }
    else {
        m_joypadState = bitSet(m_joypadState, 1);
    }

    if (input.isPressed(GLFW_KEY_UP)) {
        keyPressed(2);
    }
    else {
        m_joypadState = bitSet(m_joypadState, 2);
    }

    if (input.isPressed(GLFW_KEY_DOWN)) {
        keyPressed(3);
    }
    else {
        m_joypadState = bitSet(m_joypadState, 3);
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