#pragma once

#include <utils/UUID.hpp>

struct IDComponent
{
    IDComponent() = default;
    IDComponent(const IDComponent&) = default;

    UUID ID;
};