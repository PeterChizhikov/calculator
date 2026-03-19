#include "Checker.h"

void Checker::checkValues(MathInfo &info) {
    Logger::getInstance().debug("Validation started: check values started");
    if (std::string("+-*/^!").find(info.getOperation()) == std::string::npos) {
        Logger::getInstance().error("Validation failed: invalid operation detected");
        throw std::invalid_argument("Invalid operation!");
    }
    if (info.getOperation() == '/' && info.getSecondNum() == 0) {
        Logger::getInstance().error("Validation failed: division by zero!");
        throw std::runtime_error("Division by zero!");
    }
    Logger::getInstance().debug("Input values are valid");
}