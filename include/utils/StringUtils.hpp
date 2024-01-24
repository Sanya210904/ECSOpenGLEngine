#pragma once

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>

class StringUtils {
public:
    struct StringHash {
        using is_transparent = void;

        size_t operator()(const char* txt) const { return std::hash<std::string_view>{}(txt); }

        size_t operator()(std::string_view txt) const { return std::hash<std::string_view>{}(txt); }

        size_t operator()(const std::string& txt) const { return std::hash<std::string>{}(txt); }
    };

    template <typename... Args>
    static std::string format(const std::string& str, Args... args) {
        int size_s = std::snprintf(nullptr, 0, str.c_str(), args...) + 1;

        if (size_s <= 0)
            throw std::runtime_error("ERROR::STRING_UTILS::FORMAT");

        auto size = static_cast<size_t>(size_s);
        std::unique_ptr<char[]> buf(new char[size]);
        std::snprintf(buf.get(), size, str.c_str(), args...);

        return std::string(buf.get(), buf.get() + size - 1);
    };
};
