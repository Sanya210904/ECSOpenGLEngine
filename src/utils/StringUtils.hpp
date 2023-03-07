#pragma once

#include <functional>

class StringUtils
{
public:
    struct StringHash
    {
        using is_transparent = void;

        size_t operator()(const char* txt) const
        {
            return std::hash<std::string_view>{}(txt);
        }

        size_t operator()(std::string_view txt) const
        {
            return std::hash<std::string_view>{}(txt);
        }

        size_t operator()(const std::string& txt) const
        {
            return std::hash<std::string>{}(txt);
        }
    };
};