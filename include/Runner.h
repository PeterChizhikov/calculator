#pragma once

#include "CacheManager.h"
#include "Calculator.h"
#include "CalculatorException.h"
#include "Checker.h"
#include "DatabaseProcessor.h"
#include "Logger.h"
#include "MathInfo.h"
#include "Parser.h"
#include "Printer.h"
#include <iostream>
#include <memory>
#include <string>

class Runner {
private:
    int numberOfInputArgs;
    char **inputString;
    CacheManager cache;

public:
    Runner(int argc, char **argv);

    void run();
};
