#include "Calculator.h"

void Calculator::calculateValues(MathInfo &info) {
    Logger::getInstance().debug("Starting calculation");
    OperationResultStatus operationStatus = OperationResultStatus::ErrorInOperation;
    int result = 0;

    switch (info.operation) {
    case '+':
        operationStatus = static_cast<OperationResultStatus>(
            mathOperations::sum(info.firstNum, info.secondNum, result));
        break;
    case '-':
        operationStatus = static_cast<OperationResultStatus>(
            mathOperations::subtract(info.firstNum, info.secondNum, result));
        break;
    case '*':
        operationStatus = static_cast<OperationResultStatus>(
            mathOperations::multiply(info.firstNum, info.secondNum, result));
        break;
    case '/':
        operationStatus = static_cast<OperationResultStatus>(
            mathOperations::divide(info.firstNum, info.secondNum, result));
        break;
    case '^':
        operationStatus = static_cast<OperationResultStatus>(
            mathOperations::pow(info.firstNum, info.secondNum, result));
        break;
    case '!':
        operationStatus =
            static_cast<OperationResultStatus>(mathOperations::factorial(info.firstNum, result));
        break;
    default:
        operationStatus = OperationResultStatus::ErrorInOperation;
        break;
    }

    if (operationStatus == OperationResultStatus::SuccessfulOperation) {
        info.result = result;
        Logger::getInstance().info("Calculation finished successfully");
        return;
    }
    if (operationStatus == OperationResultStatus::ErrorInOperation) {
        Logger::getInstance().error("Calculation failed: error in operation");
        throw CalculatorException("Error in operation!", static_cast<int8_t>(operationStatus));
    }
    if (operationStatus == OperationResultStatus::Overflow) {
        Logger::getInstance().error("Calculation failed: overflow");
        throw CalculatorException("Overflow!", static_cast<int8_t>(operationStatus));
    }
    if (operationStatus == OperationResultStatus::InvalidOperation) {
        Logger::getInstance().error("Calculation failed: invalid operation");
        throw CalculatorException("Invalid operation!", static_cast<int8_t>(operationStatus));
    }
}