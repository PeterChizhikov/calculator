#include "Parser.h"

MathInfo Parser::parseValues(int argc, char** argv){
    MathInfo info;

    if (argc < 2)
        throw std::runtime_error("JSON argument missing");

    json data = json::parse(argv[1]);

    info.setFirstNum(data.value("firstNum", 0));
    info.setSecondNum(data.value("secondNum", 0));

    std::string op = data.value("operation", "&");
    info.setOperation(op[0]);

    return info;
}