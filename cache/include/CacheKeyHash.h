#pragma once

#include "CacheKey.h"

#include <cstddef>
#include <functional>

struct CacheKeyHash {
    std::size_t operator()(const CacheKey& key) const {
        const std::size_t h1 = std::hash<int>{}(key.firstNum);
        const std::size_t h2 = std::hash<int>{}(key.secondNum);
        const std::size_t h3 = std::hash<char>{}(key.operation);

        return h1 ^ (h2 << 1U) ^ (h3 << 2U);
    }
};