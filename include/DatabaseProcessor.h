#pragma once

#include "Logger.h"
#include "MathInfo.h"
#include <exception>
#include <libpq-fe.h>
#include <memory>
#include <string>

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
    void recordNewData(const MathInfo &info) const;
};
