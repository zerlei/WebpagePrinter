#pragma once
#include <QString>
#include <ctime>
#include <exception>
#include <string>
class JsonParseError : public std::exception {

  public:
    explicit JsonParseError(const QString& sourceJson, const QString& errorMsg) {
        std::time_t result     = std::time(nullptr);
        auto        error_time = std::asctime(std::localtime(&result));
        this->error_msg        = QString("source_json:\n%1\nerror_msg:\n%2\nerror_time:\n%3\n")
                              .arg(sourceJson)
                              .arg(errorMsg)
                              .arg(error_time)
                              .toStdString();
    }

    const char* what() const noexcept override { return error_msg.c_str(); }

  private:
    std::string error_msg;
};
