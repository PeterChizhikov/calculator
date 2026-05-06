#pragma once

#include "CalculationProcessor.h"
#include "Logger.h" 

#include <chrono> 
#include <string> 
#include <atomic>
#include <string>
#include <zmq.hpp>

class CalculatorServer {
public:
    explicit CalculatorServer(std::string endpoint);

    void run(const std::atomic_bool &stopRequested);
    void stop();

private:
    std::string endpoint;
    zmq::context_t context;
    CalculationProcessor processor;
};