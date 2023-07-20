#pragma once

#include <vector>

template<typename T, typename Alloc, typename Pred>
constexpr typename std::vector<T, Alloc>::size_type erase_if(std::vector<T, Alloc>& c, Pred pred)
{
    auto it = std::remove_if(c.begin(), c.end(), pred);
    auto r = std::distance(it, c.end());
    c.erase(it, c.end());
    return r;
}
