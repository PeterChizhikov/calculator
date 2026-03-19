#include "Checker.h"

void Checker::checkValues(MathInfo &info) {
    if (std::string("+-*/^!").find(info.getOperation()) == std::string::npos) {
        throw std::invalid_argument("Invalid operation!");
    }
    if (info.getOperation() == '/' && info.getSecondNum() == 0) {
        throw std::runtime_error("Division by zero!");
    }
}