#include "Runner.h"

Runner::Runner(int argc, char **argv) : numberOfInputArgs(argc), inputString(argv) {}

void Runner::run() {
    Logger::getInstance().info("Application started");
    try {
        std::unique_ptr<DatabaseProcessor> dbprocessor = std::make_unique<DatabaseProcessor>();
        MathInfo info = Parser::parseValues(numberOfInputArgs, inputString);
        Checker::checkValues(info);
        dbprocessor->recordNewData(info);
        Calculator::calculateValues(info);
        Printer::printValues(info);
        Logger::getInstance().info("Application finished successfully");
    } catch (const std::exception &e) {
        Logger::getInstance().error(std::string("Application finished with error: ") + e.what());
    }
}