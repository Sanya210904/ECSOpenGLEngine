#pragma once

#include <cstddef>

class UUID {
public:
    UUID();
    UUID(long long uuid);
    UUID(const UUID&) = default;

    operator long long() const { return m_UUID; }

private:
    long long m_UUID;
};

namespace std {
template <typename T>
struct hash;

template <>
struct hash<UUID> {
    size_t operator()(const UUID& uuid) const { return (long long)uuid; }
};
} // namespace std
