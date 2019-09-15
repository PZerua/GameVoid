/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under MIT license. Read LICENSE for more details.
*/

#include "input.h"

namespace input 
{

// Setup the keyboard singleton.
Keyboard Keyboard::m_instance;

Keyboard& Keyboard::getInstance()
{
    return Keyboard::m_instance;
}

// Input callback functions
void onKeyChange(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Keyboard::getInstance().setKeyState(key, action != GLFW_RELEASE);
}

} // namespace input
