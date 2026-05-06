#include "CalculatorServer.h"

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <zmq.hpp>

#include <atomic>
#include <chrono>
#include <string>
#include <thread>

using json = nlohmann::json;

static std::string sendRequest(const std::string &endpoint, const std::string &request) {
        zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::req);

    socket.set(zmq::sockopt::rcvtimeo, 100);
    socket.set(zmq::sockopt::sndtimeo, 100);
    socket.set(zmq::sockopt::linger, 0);

    socket.connect(endpoint);

    const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(3);

    bool sent = false;

    while (std::chrono::steady_clock::now() < deadline) {
        if (!sent) {
            zmq::message_t message(request.begin(), request.end());

            const auto sendResult = socket.send(message, zmq::send_flags::none);
            if (!sendResult.has_value()) {
                continue;
            }

            sent = true;
        }

        zmq::message_t response;
        const auto received = socket.recv(response, zmq::recv_flags::none);

        if (received.has_value()) {
            return std::string(
                static_cast<const char *>(response.data()),
                response.size()
            );
        }
    }

    throw std::runtime_error("No response from calculator service");
}

TEST(ServiceClientTest, SendsTaskAndReceivesCalculationResult) {
    const std::string endpoint = "tcp://127.0.0.1:5556";

    std::atomic_bool stopRequested{false};
    CalculatorServer server(endpoint);

    std::thread serverThread([&]() {
        server.run(stopRequested);
    });

    try {
        const std::string responseText = sendRequest(
            endpoint,
            R"({"firstNum":7,"secondNum":3,"operation":"*"})"
        );

        const json response = json::parse(responseText);

        EXPECT_EQ(response["status"], "ok");
        EXPECT_EQ(response["result"], 21);
    } catch (...) {
        stopRequested.store(true);
        server.stop();

        if (serverThread.joinable()) {
            serverThread.join();
        }

        throw;
    }

    stopRequested.store(true);
    server.stop();

    if (serverThread.joinable()) {
        serverThread.join();
    }
}