#include "Runner.h"


Runner::Runner(int argc, char** argv){
    std::cout << "Runner()\n";
    MathInfo res = Parser::parseValues(argc, argv);
    std::cout << "Parser()\n";
}

