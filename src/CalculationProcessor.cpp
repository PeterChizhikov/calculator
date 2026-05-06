#include "CalculationProcessor.h"

using json = nlohmann::json;

CalculationProcessor::CalculationProcessor() {
    loadCache();
}

void CalculationProcessor::loadCache() {
    const auto savedCalculations = db.getAllCalculations();
    
    for (const auto &info : savedCalculations) {
        cache.put(info.firstNum, info.secondNum, info.operation, info.result);
    }

    Logger::getInstance().info("Saved calculations were loaded into service cache");
}

std::string CalculationProcessor::processJsonRequest(const std::string &request) {
    std::lock_guard<std::mutex> lock(mtx);

    MathInfo info;
    std::string operationId; 

    json response;

    try{
        const json data = json::parse(request);

        info.firstNum = data.value("firstNum", 0);
        info.secondNum = data.value("secondNum", 0);

        const std::string operation = data.value("operation", "");
        if (operation.empty()) {
            throw std::invalid_argument("Operation is empty");
        }

        info.operation = operation[0];

        Checker::checkValues(info);

        if (const auto cachedResult = cache.get(info.firstNum, info.secondNum, info.operation)) {
            info.result = *cachedResult;

    }else{
            operationId = db.recordNewData(info);
            Calculator::calculateValues(info);
            cache.put(info.firstNum, info.secondNum, info.operation, info.result);
            db.recordResult(info, operationId, "ok");
    }

    response["status"] = "ok";
    response["result"] = info.result;

    } catch (const CalculatorException &e) {
        response["status"] = "error";
        response["message"] = e.what();
        response["code"] = e.getErrorCode();

        if (!operationId.empty()) {
            db.recordResult(info, operationId, e.what(), e.getErrorCode());
        }


    }catch (const std::exception &e) {
        response["status"] = "error";
        response["message"] = e.what();
        response["code"] = -1;
    }

    return response.dump();
}
