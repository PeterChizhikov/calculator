#include "Runner.h"

Runner::Runner(int argc, char **argv) : numberOfInputArgs(argc), inputString(argv) {}

void Runner::run() {
    Logger::getInstance().info("Application started");
    try {
        MathInfo info = Parser::parseValues(numberOfInputArgs, inputString);
        Checker::checkValues(info);
        Calculator::calculateValues(info);
        Printer::printValues(info);
        Logger::getInstance().info("Application finished successfully");
    } catch (const std::exception &e) {
        Logger::getInstance().error(std::string("Application finished with error: ") + e.what());
    }
}