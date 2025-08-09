#include "ModelsJson.h"
#include "PrintedPage.h"
#include "PrinterConfig.h"
#include <deque>
#include <qjsonarray.h>
#include <qjsonobject.h>
#include <string_view>
#include <utility>
struct RespBase {
    RespBase()      = delete;
    bool is_success = false;
    int  uid{-1};
};
struct RespError : RespBase {
    QString            msg{""};
    static QJsonObject toJsonObject(int uid, const QString& msg) {
        QJsonObject obj;
        obj["is_success"] = false;
        obj["uid"]        = uid;
        obj["msg"]        = msg;
        return obj;
    }
};
struct RequestAddConfig {
    static constexpr inline std::string_view method{"add_config"};
    PrinterConfig                            data{};
};
struct RespAddConfig : RespBase {
    int config_id{-1};

    static QJsonObject toJsonObject(int uid, int config_id) {

        QJsonObject obj;
        obj["is_success"] = true;
        obj["uid"]        = uid;
        obj["config_id"]  = config_id;
        return obj;
    }
};

struct RequestUpdateConfig {
    static constexpr inline std::string_view method{"update_config"};
    PrinterConfig                            data{};
};
struct RespUpdateConfig : RespBase {

    static QJsonObject toJsonObject(int uid) {
        QJsonObject obj;
        obj["is_success"] = true;
        obj["uid"]        = uid;
        return obj;
    }
};

struct RequestDelConfig {
    static constexpr inline std::string_view method{"del_config"};
    int                                      data{};
};
struct RespDelConfig : RespBase {
    static QJsonObject toJsonObject(int uid) {
        QJsonObject obj;
        obj["is_success"] = true;
        obj["uid"]        = uid;
        return obj;
    }
};
struct RequestGetAllConfigs {
    static constexpr inline std::string_view method{"get_all_configs"};
};
struct RespGetAllConfigs : RespBase {
    std::deque<PrinterConfig> data{};
    static QJsonObject        toJsonObject(int uid, const std::deque<PrinterConfig>& configs) {
        QJsonObject obj;
        obj["is_success"] = true;
        obj["uid"]        = uid;
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
    int                                      page_index{1};
    int                                      page_size{10};
};

struct RespGetPagesDesc : RespBase {
    std::deque<PrinterConfig> data{};
    static QJsonObject        toJsonObject(int uid, const std::deque<PrintedPage>& configs) {
        QJsonObject obj;
        obj["is_success"] = true;
        obj["uid"]        = uid;
        QJsonArray arr;
        for (auto& config : configs) {
            arr.append(toPrintedPageJson(config));
        }
        obj["data"] = arr;
        return obj;
    }
};