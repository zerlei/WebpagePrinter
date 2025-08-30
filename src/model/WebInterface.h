#pragma once
#include "ModelsJson.h"
#include "PrintedPage.h"
#include "PrinterConfig.h"
#include <QJsonObject>
#include <QPrinterInfo>
#include <deque>
#include <optional>
#include <qjsonarray.h>
#include <qjsonobject.h>
#include <string_view>
struct RespBase {
    RespBase()                    = delete;
    bool               is_success = false;
    std::optional<int> uid;
};
struct RespError : RespBase {
    QString            msg{""};
    static QJsonObject toJsonObject(std::optional<int> uid, const QString& msg) {
        QJsonObject obj;
        obj["is_success"] = false;
        if (uid.has_value()) {
            obj["uid"] = uid.value();
        }
        obj["msg"] = msg;
        return obj;
    }
};
struct RequestAddConfig {
    static constexpr inline std::string_view method{"add_config"};
    std::optional<int>                       uid;
    PrinterConfig                            data{};
};
struct RespAddConfig : RespBase {
    int config_id{-1};

    static QJsonObject toJsonObject(std::optional<int> uid, int config_id) {

        QJsonObject obj;
        obj["is_success"] = true;
        if (uid.has_value()) {
            obj["uid"] = uid.value();
        }
        obj["config_id"] = config_id;
        return obj;
    }
};

struct RequestUpdateConfig {
    static constexpr inline std::string_view method{"update_config"};
    std::optional<int>                       uid;
    PrinterConfig                            data{};
};
struct RespUpdateConfig : RespBase {

    static QJsonObject toJsonObject(std::optional<int> uid) {
        QJsonObject obj;
        obj["is_success"] = true;
        if (uid.has_value()) {
            obj["uid"] = uid.value();
        }
        return obj;
    }
};

struct RequestDelConfig {
    static constexpr inline std::string_view method{"del_config"};
    std::optional<int>                       uid;
    int                                      data{};
};
struct RespDelConfig : RespBase {
    static QJsonObject toJsonObject(std::optional<int> uid) {
        QJsonObject obj;
        obj["is_success"] = true;
        if (uid.has_value()) {
            obj["uid"] = uid.value();
        }
        return obj;
    }
};
struct RequestGetAllConfigs {
    static constexpr inline std::string_view method{"get_all_configs"};
    std::optional<int>                       uid;
};
struct RespGetAllConfigs : RespBase {
    std::deque<PrinterConfig> data{};
    static QJsonObject        toJsonObject(std::optional<int>               uid,
                                           const std::deque<PrinterConfig>& configs) {
        QJsonObject obj;
        obj["is_success"] = true;
        if (uid.has_value()) {
            obj["uid"] = uid.value();
        }
        QJsonArray arr;
        for (auto& config : configs) {
            arr.append(toPrinterConfigJson(config));
        }
        obj["data"] = arr;
        return obj;
    }
};

struct RequestGetPagesDesc {
    static constexpr inline std::string_view method{"get_pages_desc"};
    std::optional<int>                       uid;
    int                                      page_index{1};
    int                                      page_size{10};
};

struct RespGetPagesDesc : RespBase {
    std::deque<PrinterConfig> data{};
    static QJsonObject        toJsonObject(std::optional<int>             uid,
                                           const std::deque<PrintedPage>& configs) {
        QJsonObject obj;
        obj["is_success"] = true;
        if (uid.has_value()) {
            obj["uid"] = uid.value();
        }
        QJsonArray arr;
        for (auto& config : configs) {
            arr.append(toPrintedPageJson(config));
        }
        obj["data"] = arr;
        return obj;
    }
};

struct RequestGetPrintersInfo {
    static constexpr inline std::string_view method{"get_printers_info"};
    std::optional<int>                       uid;
};
struct PrinterInfo {
    QString        printer_name{""};
    QList<QString> supported_paper_names{};
};
struct RespGetPrintersInfo : RespBase {

    QList<PrinterInfo> data{};

    static QJsonObject toJsonObject(std::optional<int> uid, const QList<QPrinterInfo>& printers) {
        QJsonObject obj;
        obj["is_success"] = true;
        if (uid.has_value()) {
            obj["uid"] = uid.value();
        }
        QJsonArray arr;
        for (auto& printer : printers) {
            QJsonObject printer_obj;
            printer_obj["printer_name"] = printer.printerName();
            QJsonArray paper_names_arr;
            for (auto& paper_name : printer.supportedPageSizes()) {
                paper_names_arr.append(paper_name.name());
            }
            printer_obj["supported_paper_names"] = paper_names_arr;
            arr.append(printer_obj);
        }
        obj["data"] = arr;
        return obj;
    }
};

struct RequestPrintPage {
    static constexpr inline std::string_view method{"print_page"};
    std::optional<int>                       uid;
    PrintedPage                              data{};
};

struct RespPrintPage : RespBase {
    QString            file_path{""};
    static QJsonObject toJsonObject(std::optional<int> uid, const QString& file_path) {
        QJsonObject obj;
        obj["is_success"] = true;
        if (uid.has_value()) {
            obj["uid"] = uid.value();
        }
        obj["data"] = file_path;
        return obj;
    }
};