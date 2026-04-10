#include "CalculatorException.h"

CalculatorException::CalculatorException(std::string msg, int code)
    : message(std::move(msg)), errorCode(code) {}

const char *CalculatorException::what() const noexcept {
    return message.c_str();
}

int CalculatorException::getErrorCode() const noexcept {
    return errorCode;
}