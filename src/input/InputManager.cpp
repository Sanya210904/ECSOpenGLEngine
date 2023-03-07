#include "InputManager.hpp"

#include <utils/Utils.hpp>

InputManager::InputManager()
{
    m_active = true;
}

InputManager::~InputManager()
{
    m_active = false;
}

void InputManager::registerActionCallback(const std::string& actionName, const ActionCallback& callback)
{
    m_actionCallbacks[actionName].emplace_back(callback);
}

void InputManager::removeActionCallback(const std::string& actionName, const std::string& callbackRef)
{
    erase_if(m_actionCallbacks[actionName], [callbackRef](ActionCallback callback){
        return callback.ref == callbackRef;
    });
}

void InputManager::mapInputToAction(InputKey key, const std::string& action)
{
    m_inputActionMapping[key].emplace(action);
}

void InputManager::unmapInputFromAction(InputKey key, const std::string& action)
{
    m_inputActionMapping[key].erase(action);
}

void InputManager::processInput()
{
    std::vector<ActionEvent> events {};
    for (auto& device : m_devices)
    {
        auto newState = device.stateFunc(device.index);

        for (auto& [key, state] : newState)
        {
            float newVal = state.value;
            auto debugVal = device.currentState;

            if (device.currentState[key].value != newVal)
            {
                auto generatedEvents = genActionEvent(device.index, key, newVal);
                events.insert(events.end(), generatedEvents.begin(), generatedEvents.end());
                device.currentState[key].value = newVal;
            }
        }
    }

    for (auto& event : events)
    {
        propagateActionEvent(event);
    }
}

std::vector<InputManager::ActionEvent> InputManager::genActionEvent(int deviceIndex, InputKey key, float newVal)
{
    std::vector<ActionEvent> actionEvents {};
    auto& actions = m_inputActionMapping[key];
    InputSource source = getInputSourceFromKey(key);
    
    for (auto& action : actions)
    {
        actionEvents.emplace_back([&](){
            ActionEvent _;
            _.actionName = action;
            _.source = source;
            _.sourceIndex = deviceIndex;
            _.value = newVal;
            return _;
        }());
    }

    return actionEvents;
}

void InputManager::propagateActionEvent(const ActionEvent& event)
{
    if (m_actionCallbacks[event.actionName].empty()) return;
    for (size_t i = m_actionCallbacks[event.actionName].size() - 1; i >= 0; --i)
    {
        auto& actionCallback = m_actionCallbacks[event.actionName][i];
        if(actionCallback.func(event.source, event.sourceIndex, event.value)) break;
    }
}

void InputManager::regDevice(const InputDevice& device)
{
    m_devices.emplace_back(device);
}

void InputManager::remDevice(const InputDeviceType& type, int inputIndex)
{
    erase_if(m_devices, [type, inputIndex](InputDevice device)
    {
        return device.type == type && device.index == inputIndex;
    });
}
