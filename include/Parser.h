#include "MathInfo.h"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Parser {
private:
    /* data */
public:
    static MathInfo parseValues(int argc, char **argv);
    void printHelp();
};
