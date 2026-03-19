#pragma once

class MathInfo {
private:
    int firstNum = {};
    int secondNum = {};
    char operation = {};
    int result = {};

public:
    int getFirstNum() const noexcept {
        return firstNum;
    }
    int getSecondNum() const noexcept {
        return secondNum;
    }
    char getOperation() const noexcept {
        return operation;
    }
    int getResult() const noexcept {
        return result;
    }

    void setFirstNum(int value) noexcept {
        firstNum = value;
    }
    void setSecondNum(int value) noexcept {
        secondNum = value;
    }
    void setOperation(char value) noexcept {
        operation = value;
    }
    void setResult(int value) noexcept {
        result = value;
    }
};