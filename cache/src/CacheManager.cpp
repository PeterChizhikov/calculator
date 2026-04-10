#include "CacheManager.h"

auto CacheManager::normalizeKey(int firstNum, int secondNum, char operation) -> CacheKey {
    if (operation == '+' || operation == '*') {
        if (firstNum > secondNum) {
            std::swap(firstNum, secondNum);
        }
    }

    return CacheKey{firstNum, secondNum, operation};
}

auto CacheManager::contains(int firstNum, int secondNum, char operation) const -> bool {
    const CacheKey key = normalizeKey(firstNum, secondNum, operation);
    return cacheHandler.find(key) != cacheHandler.end();
}

auto CacheManager::get(int firstNum, int secondNum, char operation) const -> std::optional<int> {
    const CacheKey key = normalizeKey(firstNum, secondNum, operation);
    const auto it = cacheHandler.find(key);

    if (it != cacheHandler.end()) {
        return it->second;
    }

    return std::nullopt;
}

void CacheManager::put(int firstNum, int secondNum, char operation, int result) {
    const CacheKey key = normalizeKey(firstNum, secondNum, operation);
    cacheHandler[key] = result;
}

auto CacheManager::size() const -> std::size_t {
    return cacheHandler.size();
}

void CacheManager::clear() {
    cacheHandler.clear();
}