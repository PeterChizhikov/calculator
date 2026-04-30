#include "Parser.h"

MathInfo Parser::parseValues(int argc, char **argv) {
    MathInfo info;

    if (argc < 2) {
        Logger::getInstance().error("JSON argument is missing");
        throw std::runtime_error("JSON argument missing");
    }
    Logger::getInstance().debug(argv[1]);
    json data = json::parse(argv[1]);

    info.firstNum = data.value("firstNum", 0);
    info.secondNum = data.value("secondNum", 0);

    std::string operation = data.value("operation", "&");
    info.operation = operation[0];

    Logger::getInstance().info("Input JSON parsed successfully");

    return info;
}