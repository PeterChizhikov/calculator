#pragma once

#include "Logger.h"
#include "MathInfo.h"
#include <exception>
#include <libpq-fe.h>
#include <memory>
#include <string>
#include <vector>

class DatabaseProcessor {
private:
    std::unique_ptr<PGconn, decltype(&PQfinish)> connection{nullptr, &PQfinish};
    std::string host = "localhost";
    std::string port = "5432";
    std::string dbname = "calculator";
    std::string user = "student";
    std::string password = "pass";
    std::string returnConnectionString() const;
    void createTable();

public:
    explicit DatabaseProcessor();
    /**
     * @brief Сохраняет новую запись в таблицу calculations.
     * @param info Данные вычисления.
     * @return UUID новой записи.
     */
    [[nodiscard]] std::string recordNewData(const MathInfo &info) const;
    /**
     * @brief Вносит результат операции в таблицу calculations.
     * @param info Данные вычисления.
     * @param operationKey Ключ операции, для которой будет записан результат.
     * @param message Сообщение по результатам операции
     */
    void recordResult(const MathInfo &info, const std::string &operationKey,
                      std::string message = "Error", int errorCode = 0) const;

    std::vector<MathInfo> getAllCalculations() const;
};
