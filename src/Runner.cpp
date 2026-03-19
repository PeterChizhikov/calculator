#include "Runner.h"

Runner::Runner(int argc, char **argv) : numberOfInputArgs(argc), inputString(argv) {}

void Runner::run() {
    try {
        MathInfo info = Parser::parseValues(numberOfInputArgs, inputString);
        Checker::checkValues(info);
        Calculator::calculateValues(info);
        Printer::printValues(info);
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
}