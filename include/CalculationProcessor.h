#pragma once 

#include "CacheManager.h"
#include "DatabaseProcessor.h"
#include "MathInfo.h"
#include "Calculator.h"
#include "CalculatorException.h"
#include "Checker.h"
#include "Logger.h"

#include <nlohmann/json.hpp>
#include <stdexcept>
#include <mutex>
#include <string> 

class CalculationProcessor{
    public:
        CalculationProcessor();

        std::string processJsonRequest(const std::string &request);

    private:
        DatabaseProcessor db;
        CacheManager cache;
        std::mutex mtx;

        void loadCache();
};