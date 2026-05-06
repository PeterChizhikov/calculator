#include "Logger.h"
#include "Runner.h"
#include <exception>
#include <string>

int main(int argc, char **argv) {
    try {
        Runner runApp(argc, argv);
        runApp.run();
    } catch (const std::exception &e) {
        Logger::getInstance().error(std::string("Program didn't finish! Error occured: ") +
                                    e.what());
        return 1;
    }
    return 0;
}
