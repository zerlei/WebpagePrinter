#pragma once
#include <QString>
#include <ctime>
#include <exception>
#include <string>
class SqliteOpError : public std::exception {

  public:
    explicit SqliteOpError(const QString& errorMsg) {
        std::time_t result     = std::time(nullptr);
        auto        error_time = std::asctime(std::localtime(&result));
        this->error_msg        = QString("error_msg:\n%1\nerror_time:\n%2\n")
                              .arg(errorMsg)
                              .arg(error_time)
                              .toStdString();
    }

    const char* what() const noexcept override { return error_msg.c_str(); }

  private:
    std::string error_msg;
};