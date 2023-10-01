#pragma once

#include <functional>
#include <unordered_map>

#include "InputKey.hpp"

enum class InputDeviceType
{
    KEYBOARD,
    MOUSE,
    GAMEPAD
};

struct InputDeviceState
{
    float value;
};

using InputDeviceStateCallbackFunc = std::function<std::unordered_map<InputKey, InputDeviceState>(int)>;
struct InputDevice
{
    InputDeviceType type;
    int index;
    InputDeviceStateCallbackFunc stateFunc;
    std::unordered_map<InputKey, InputDeviceState> currentState;
};
