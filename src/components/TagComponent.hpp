#pragma once

#include <string>

struct TagComponent
{
    TagComponent() = default;
    TagComponent(const TagComponent&) = default;
    TagComponent(const std::string& tag)
        :
        tag(tag) { }

    std::string tag;
};