#pragma once

#include <unordered_map>

#include "InputKey.hpp"
#include "InputDevice.hpp"

struct GLFWwindow;
class Input
{
public:
    std::unordered_map<InputKey, InputDeviceState> getKeyboardState(int index) { return m_keyboardState; }
    std::unordered_map<InputKey, InputDeviceState> getMouseState(int index) { return m_mouseState; }
    std::unordered_map<InputKey, InputDeviceState> getGamepadState(int index) { return m_gamepadStates[index]; }

    void updateKeyboardState(int key, float value);
    void updateMouseState(int button, float value);
    void updateCursorState(GLFWwindow* window);

private:
    static InputKey keyToInputKey(int key);
    static InputKey mouseButtonToInputKey(int button);

    std::unordered_map<InputKey, InputDeviceState> m_keyboardState {};
    std::unordered_map<InputKey, InputDeviceState> m_mouseState {};

    std::unordered_map<int, std::unordered_map<InputKey, InputDeviceState>> m_gamepadStates {};
};