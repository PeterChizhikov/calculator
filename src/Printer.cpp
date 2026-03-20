#include "Printer.h"

void Printer::printValues(MathInfo &info) {
    Logger::getInstance().debug("Printing calculation result");
    std::cout << "Result: " << info.result << std::endl;
}
