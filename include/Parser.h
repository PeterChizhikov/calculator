#include <nlohmann/json.hpp>
#include "MathInfo.h"

using json = nlohmann::json;

class Parser
{
private:
    /* data */
public:
    static MathInfo parseValues(int argc, char** argv);
    void printHelp();
};
