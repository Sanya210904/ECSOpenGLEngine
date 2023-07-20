#pragma once

#include <graphics/Model.hpp>

struct ModelComponent
{
    ModelComponent() = default;
    ModelComponent(const ModelComponent&) = default;
    ModelComponent(const Model& model)
        :
        model(model) { }

    Model model;
};
