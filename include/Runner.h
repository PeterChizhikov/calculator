#pragma once

#include "Calculator.h"
#include "Checker.h"
#include "MathInfo.h"
#include "Parser.h"
#include "Printer.h"
#include <iostream>

class Runner {
private:
    int numberOfInputArgs;
    char **inputString;

public:
    Runner(int argc, char **argv);

    void run();
};
