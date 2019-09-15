/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under MIT license. Read LICENSE for more details.
*/

#pragma once
#include <GLFW/glfw3.h>

namespace input 
{
    // Callbacks definition
    void onKeyChange(GLFWwindow* window, int key, int scancode, int action, int mods);

class Keyboard
{
public:
    static Keyboard& getInstance();

    inline void setKeyState(unsigned int key, bool state)
    {
        m_keys[key] = state;
    }

    inline bool isPressed(unsigned int key) const
    {
        return m_keys[key];
    }

private:
    Keyboard()
    {
        for (int i = 0; i < GLFW_KEY_LAST; ++i)
            m_keys[i] = false;
    }

private:
    static Keyboard m_instance;

    // Current state of the keyboard
    bool m_keys[GLFW_KEY_LAST];
};

} // namespace input