#include "CalculatorServer.h"
#include "Logger.h"

#include <atomic>
#include <csignal>
#include <exception>
#include <pthread.h>
#include <string>
#include <thread>

int main(int argc, char **argv) {
    const std::string endpoint = argc > 1 ? argv[1] : "tcp://127.0.0.1:5555";

    sigset_t signalSet;
    sigemptyset(&signalSet);
    sigaddset(&signalSet, SIGINT);
    sigaddset(&signalSet, SIGTERM);

    pthread_sigmask(SIG_BLOCK, &signalSet, nullptr);

    std::atomic_bool stopRequested{false};

    try {
        CalculatorServer server(endpoint);

        std::thread signalThread([&]() {
            int signal = 0;
            sigwait(&signalSet, &signal);

            Logger::getInstance().info("Stop signal received");
            stopRequested.store(true);
            server.stop();
        });

        std::thread workerThread([&]() {
            server.run(stopRequested);
        });

        workerThread.join();
        signalThread.join();
    } catch (const std::exception &e) {
        Logger::getInstance().error(std::string("Service failed: ") + e.what());
        return 1;
    }

    return 0;
}