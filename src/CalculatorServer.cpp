#include "CalculatorServer.h" 



CalculatorServer::CalculatorServer(std::string endpoint)
    : endpoint(std::move(endpoint)), context(1) {}

void CalculatorServer::run(const std::atomic_bool &stopRequested) {
    zmq::socket_t socket(context, zmq::socket_type::rep);

    socket.set(zmq::sockopt::rcvtimeo, 100);
    socket.set(zmq::sockopt::sndtimeo, 100);
    socket.set(zmq::sockopt::linger, 0);

    socket.bind(endpoint);

    Logger::getInstance().info("Calculator service started on endpoint: " + endpoint);

    while (!stopRequested.load()) {
        try {
            zmq::message_t request;

            const auto received = socket.recv(request, zmq::recv_flags::none);
            if (!received.has_value()) {
                continue;
            }

            const std::string requestText(
                static_cast<const char *>(request.data()),
                request.size()
            );

            const std::string responseText = processor.processJsonRequest(requestText);

            zmq::message_t response(responseText.begin(), responseText.end());
            socket.send(response, zmq::send_flags::none);
        } catch (const zmq::error_t &e) {
            if (stopRequested.load()) {
                break;
            }

            Logger::getInstance().error(std::string("ZeroMQ error: ") + e.what());
        } catch (const std::exception &e) {
            Logger::getInstance().error(std::string("Server error: ") + e.what());
        }
    }

    Logger::getInstance().info("Calculator service stopped");
}

void CalculatorServer::stop() {
    context.shutdown();
}

