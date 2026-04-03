#pragma once

struct CacheKey {
    int firstNum{};
    int secondNum{};
    char operation{};

    bool operator==(const CacheKey& other) const {
        return firstNum == other.firstNum &&
               secondNum == other.secondNum &&
               operation == other.operation;
    }
};