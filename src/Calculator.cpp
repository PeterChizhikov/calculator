#include "Calculator.h"

void Calculator::calculateValues(MathInfo &info) {
    Logger::getInstance().debug("Starting calculation");
    int operationStatus = -1;
    int result = 0;

    switch (info.getOperation()) {
    case '+':
        operationStatus = mathOperations::sum(info.getFirstNum(), info.getSecondNum(), result);
        break;
    case '-':
        operationStatus = mathOperations::subtract(info.getFirstNum(), info.getSecondNum(), result);
        break;
    case '*':
        operationStatus = mathOperations::multiply(info.getFirstNum(), info.getSecondNum(), result);
        break;
    case '/':
        operationStatus = mathOperations::divide(info.getFirstNum(), info.getSecondNum(), result);
        break;
    case '^':
        operationStatus = mathOperations::pow(info.getFirstNum(), info.getSecondNum(), result);
        break;
    case '!':
        operationStatus = mathOperations::factorial(info.getFirstNum(), result);
        break;
    default:
        operationStatus = -1;
        break;
    }

    if (operationStatus == 0) {
        info.setResult(result);
        Logger::getInstance().info("Calculation finished successfully");
        return;
    }
    if (operationStatus == -1) {
        Logger::getInstance().error("Calculation failed: error in operation");
        throw std::runtime_error("Error in operation!");
    }
    if (operationStatus == -2) {
        Logger::getInstance().error("Calculation failed: overflow");
        throw std::overflow_error("Overflow!");
    }
    if (operationStatus == -3) {
        Logger::getInstance().error("Calculation failed: invalid operation");
        throw std::runtime_error("Invalid operation!");
    }
}