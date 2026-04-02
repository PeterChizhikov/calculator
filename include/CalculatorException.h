#pragma once

#include <exception>
#include <string>

class CalculatorException : public std::exception {
private:
    std::string message;
    int errorCode;

public:
    CalculatorException(std::string msg, int code);

    const char *what() const noexcept override;
    int getErrorCode() const noexcept;
};