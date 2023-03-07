#include "Input.hpp"

#include <GLFW/glfw3.h>
#include <string>

void Input::updateKeyboardState(int key, float value)
{
    InputKey iKey = keyToInputKey(key);

    m_keyboardState[iKey].value = value;
}

void Input::updateMouseState(int button, float value)
{
    InputKey iKey = mouseButtonToInputKey(button);

    m_mouseState[iKey].value = value;
}

void Input::updateCursorState(GLFWwindow* window)
{
    float lastX = m_mouseState[InputKey::MOUSE_X].value;
    float lastY = m_mouseState[InputKey::MOUSE_Y].value;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    m_mouseState[InputKey::MOUSE_OFFSET_X].value = static_cast<float>(xpos) - lastX;
    m_mouseState[InputKey::MOUSE_OFFSET_Y].value = static_cast<float>(ypos) - lastY;
    m_mouseState[InputKey::MOUSE_X].value = static_cast<float>(xpos);
    m_mouseState[InputKey::MOUSE_Y].value = static_cast<float>(ypos);
}

InputKey Input::keyToInputKey(int key)
{
    // Dangerous as if there is no Enum value for key code, it can lead to unexpected consequences
    return static_cast<InputKey>(key);
}

InputKey Input::mouseButtonToInputKey(int button)
{
    // Dangerous as if there is no Enum value for key code, it can lead to unexpected consequences
    return static_cast<InputKey>(
        static_cast<int>(InputKey::MOUSE_B1) + button
    );
}
