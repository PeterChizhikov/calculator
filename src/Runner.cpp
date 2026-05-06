#include "Runner.h"

Runner::Runner(int argc, char **argv) : numberOfInputArgs(argc), inputString(argv) {}

void Runner::run() {
    Logger::getInstance().info("Application started");
    std::string operationId;
    std::unique_ptr<DatabaseProcessor> dbprocessor;
    MathInfo info;
    try {
        dbprocessor = std::make_unique<DatabaseProcessor>();
        info = Parser::parseValues(numberOfInputArgs, inputString);
        Checker::checkValues(info);
        const std::vector<MathInfo> savedCalculations = dbprocessor->getAllCalculations();
        for (const auto &savedInfo : savedCalculations) {
            cache.put(savedInfo.firstNum, savedInfo.secondNum, savedInfo.operation,
                      savedInfo.result);
        }
        Logger::getInstance().info("Saved calculations were loaded into cache");
        if (const auto cachedResult = cache.get(info.firstNum, info.secondNum, info.operation)) {
            info.result = *cachedResult;
            Logger::getInstance().info("Result was taken from cache");
        } else {
            operationId = dbprocessor->recordNewData(info);
            Calculator::calculateValues(info);
            cache.put(info.firstNum, info.secondNum, info.operation, info.result);
            Logger::getInstance().info("Result was calculated and saved to cache");
            dbprocessor->recordResult(info, operationId, "ok");
        }
        Printer::printValues(info);
        Logger::getInstance().info("Application finished successfully");
    } catch (const std::exception &e) {
    Logger::getInstance().error(std::string("Application finished with error: ") + e.what());

    if (!operationId.empty()) {
        dbprocessor->recordResult(info, operationId, e.what(), -1);
    }
}
}