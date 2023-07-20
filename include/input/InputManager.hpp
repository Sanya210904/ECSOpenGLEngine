#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <functional>
#include <string>

#include "InputDevice.hpp"

class InputManager
{
public:
    InputManager();
    ~InputManager();

    using ActionCallbackFunc = std::function<bool(InputSource, int, float)>;
    struct ActionCallback
    {
        std::string ref;
        ActionCallbackFunc func;
    };

    void registerActionCallback(const std::string& actionName, const ActionCallback& callback);
    void removeActionCallback(const std::string& actionName, const std::string& callbackRef);

    void regDevice(const InputDevice& device);
    void remDevice(const InputDeviceType& type, int inputIndex);

    void mapInputToAction(InputKey key, const std::string& action);
    void unmapInputFromAction(InputKey key, const std::string& action);

private:
    struct ActionEvent
    {
        std::string actionName;
        InputSource source;
        int sourceIndex;
        float value;
    };

    friend class App;
    void processInput();
    std::vector<ActionEvent> genActionEvent(int deviceIndex, InputKey key, float newVal);
    void propagateActionEvent(const ActionEvent& event);

    bool m_active = false;
    std::unordered_map<InputKey, std::unordered_set<std::string>> m_inputActionMapping {};
    std::unordered_map<std::string, std::vector<ActionCallback>> m_actionCallbacks {};
    std::vector<InputDevice> m_devices;
};