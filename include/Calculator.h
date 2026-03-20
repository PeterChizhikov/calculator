#pragma once

#include "Logger.h"
#include "MathInfo.h"
#include "mathFunctions.h"
#include <stdexcept>
#include <string>

enum class OperationResultStatus : int8_t {
    SuccessfulOperation = 0,
    ErrorInOperation = -1,
    Overflow = -2,
    InvalidOperation = -3
};

class Calculator {
public:
    static void calculateValues(MathInfo &info);
};
