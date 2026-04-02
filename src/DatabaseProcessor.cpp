#include "DatabaseProcessor.h"

DatabaseProcessor::DatabaseProcessor() {
    connection = std::unique_ptr<PGconn, decltype(&PQfinish)>(
        PQconnectdb(returnConnectionString().c_str()), &PQfinish);

    if (PQstatus(connection.get()) != CONNECTION_OK) {
        const std::string errorMessage = PQerrorMessage(connection.get());
        Logger::getInstance().error("Database connection failed: " + errorMessage);
        throw std::runtime_error("Database connection failed: " + errorMessage);
    }
    Logger::getInstance().info("Database connected successfully");
    createTable();
}

std::string DatabaseProcessor::returnConnectionString() const {
    return "host=" + host + " port=" + port + " dbname=" + dbname + " user=" + user +
           " password=" + password;
};

void DatabaseProcessor::createTable() {
    const std::string enableExtensionQuery = "CREATE EXTENSION IF NOT EXISTS pgcrypto;";

    PGresult *extRawResult = PQexec(connection.get(), enableExtensionQuery.c_str());

    if (!extRawResult) {
        throw std::runtime_error("PQexec returned nullptr while enabling pgcrypto");
    }

    std::unique_ptr<PGresult, decltype(&PQclear)> extResult(extRawResult, &PQclear);

    if (PQresultStatus(extResult.get()) != PGRES_COMMAND_OK) {
        const std::string errorMessage = PQerrorMessage(connection.get());
        Logger::getInstance().error("Enable pgcrypto failed: " + errorMessage);
        throw std::runtime_error("Enable pgcrypto failed: " + errorMessage);
    }

    const std::string createTableQuery = "CREATE TABLE IF NOT EXISTS calculations ("
                                         "id UUID PRIMARY KEY DEFAULT gen_random_uuid(), "
                                         "number1 INTEGER NOT NULL, "
                                         "number2 INTEGER NOT NULL, "
                                         "operation CHAR(1) NOT NULL, "
                                         "result INTEGER, "
                                         "status INTEGER NOT NULL, "
                                         "message TEXT"
                                         ");";

    PGresult *rawResult = PQexec(connection.get(), createTableQuery.c_str());

    if (!rawResult) {
        throw std::runtime_error("PQexec returned nullptr");
    }

    std::unique_ptr<PGresult, decltype(&PQclear)> result(rawResult, &PQclear);

    if (PQresultStatus(result.get()) != PGRES_COMMAND_OK) {
        const std::string errorMessage = PQerrorMessage(connection.get());
        Logger::getInstance().error("Create table failed: " + errorMessage);
        throw std::runtime_error("Create table failed: " + errorMessage);
    }

    Logger::getInstance().info("Table 'calculations' is ready");
}

std::string DatabaseProcessor::recordNewData(const MathInfo &info) const {
    const std::string insertQuery =
        "INSERT INTO calculations (number1, number2, operation, result, status, message) "
        "VALUES ($1, $2, $3, $4, $5, $6) "
        "RETURNING id;";

    const std::string firstNumStr = std::to_string(info.firstNum);
    const std::string secondNumStr = std::to_string(info.secondNum);
    const std::string operationStr(1, info.operation);
    const std::string resultStr = std::to_string(info.result);
    const std::string statusStr = "-1";
    const std::string messageStr = "Uknown";

    const char *paramValues[6] = {firstNumStr.c_str(), secondNumStr.c_str(), operationStr.c_str(),
                                  resultStr.c_str(),   statusStr.c_str(),    messageStr.c_str()};

    PGresult *rawResult = PQexecParams(connection.get(), insertQuery.c_str(),
                                       6, // количество параметров
                                       nullptr, // типы параметров PostgreSQL определит сам
                                       paramValues,
                                       nullptr, // длины параметров
                                       nullptr, // форматы параметров
                                       0 // все параметры в текстовом формате
    );

    if (!rawResult) {
        Logger::getInstance().error("PQexecParams returned nullptr");
        throw std::runtime_error("PQexecParams returned nullptr");
    }

    std::unique_ptr<PGresult, decltype(&PQclear)> result(rawResult, &PQclear);
    if (PQresultStatus(result.get()) != PGRES_TUPLES_OK) {
        const std::string errorMessage = PQerrorMessage(connection.get());
        Logger::getInstance().error("Insert failed: " + errorMessage);
        throw std::runtime_error("Insert failed: " + errorMessage);
    }

    if (PQntuples(result.get()) != 1 || PQnfields(result.get()) != 1) {
        Logger::getInstance().error("Insert succeeded but returned invalid UUID result");
        throw std::runtime_error("Insert succeeded but returned invalid UUID result");
    }

    const std::string insertedId = PQgetvalue(result.get(), 0, 0);
    Logger::getInstance().info("New calculation was inserted successfully with id: " + insertedId);
    return insertedId;
}

void DatabaseProcessor::recordResult(const MathInfo &info, const std::string &operationKey,
                                     std::string message, int errorCode) const {
    const std::string updateQuery = "UPDATE calculations "
                                    "SET result = $1, status = $2, message = $3 "
                                    "WHERE id = $4;";

    const std::string resultStr = std::to_string(info.result);
    const std::string statusStr = std::to_string(errorCode);
    const std::string messageStr = message;

    const char *paramValues[4] = {resultStr.c_str(), statusStr.c_str(), messageStr.c_str(),
                                  operationKey.c_str()};

    PGresult *rawResult = PQexecParams(connection.get(), updateQuery.c_str(), 4, nullptr,
                                       paramValues, nullptr, nullptr, 0);

    if (!rawResult) {
        Logger::getInstance().error("PQexecParams returned nullptr");
        throw std::runtime_error("PQexecParams returned nullptr");
    }

    std::unique_ptr<PGresult, decltype(&PQclear)> result(rawResult, &PQclear);

    if (PQresultStatus(result.get()) != PGRES_COMMAND_OK) {
        const std::string errorMessage = PQerrorMessage(connection.get());
        Logger::getInstance().error("Update failed: " + errorMessage);
        throw std::runtime_error("Update failed: " + errorMessage);
    }

    if (PQcmdTuples(result.get()) == nullptr || std::string(PQcmdTuples(result.get())) != "1") {
        Logger::getInstance().error("Update failed: no row found for id = " + operationKey);
        throw std::runtime_error("Update failed: no row found for id = " + operationKey);
    }

    Logger::getInstance().info("Calculation was updated successfully for id: " + operationKey);
}