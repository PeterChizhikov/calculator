#pragma once

#include "CacheKey.h"
#include "CacheKeyHash.h"


#include <optional>
#include <algorithm>
#include <unordered_map>

class CacheManager {
private:
    std::unordered_map<CacheKey, int, CacheKeyHash> cacheHandler;
    static auto normalizeKey(int firstNum, int secondNum, char operation) -> CacheKey;

public:
    CacheManager() = default;
    auto contains(int firstNum, int secondNum, char operation) const -> bool;
    auto get(int firstNum, int secondNum, char operation) const -> std::optional<int>;
    void put(int firstNum, int secondNum, char operation, int result);
    auto size() const -> std::size_t;
    void clear();
};