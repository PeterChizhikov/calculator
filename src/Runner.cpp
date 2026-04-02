#include "Runner.h"

Runner::Runner(int argc, char **argv) : numberOfInputArgs(argc), inputString(argv) {}

void Runner::run() {
    Logger::getInstance().info("Application started");
    std::string operationId;
    std::unique_ptr<DatabaseProcessor> dbprocessor = std::make_unique<DatabaseProcessor>();
    MathInfo info;
    try {
        info = Parser::parseValues(numberOfInputArgs, inputString);
        Checker::checkValues(info);
        operationId = dbprocessor->recordNewData(info);
        Calculator::calculateValues(info);
        dbprocessor->recordResult(info, operationId, "ok");
        Printer::printValues(info);
        Logger::getInstance().info("Application finished successfully");
    } catch (const CalculatorException &e) {
        Logger::getInstance().error(std::string("Application finished with error: ") + e.what());
        dbprocessor->recordResult(info, operationId,
                                  std::string("Application finished with error: ") + e.what(),
                                  e.getErrorCode());
    }
}