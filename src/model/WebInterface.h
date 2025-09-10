#pragma once
/**
 * @file WebInterface.h
 * @brief request response 接口数据格式
 * @version 0.1
 * @date 2025-09-06
 * @copyright Copyright (c) 2025
 * 
 */
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
/**
 * @brief base response,所有返回均包含此类成员
 * 
 */
struct RespBase {
    RespBase()                    = delete;
    /**
     * @brief 是否成功
     * 
     */
    bool               is_success = false;
    /**
     * @brief uid,websocket 的消息request 和 resp 不限制对应关系,uid 保证了他们的联系, 也就是 uid 相同 的 request 和 response 对应
     * 由request端生成
     * 
     */
    std::optional<int> uid;
};

/**
 * @brief 所有接口的错误返回
 * 
 */
struct RespError : RespBase {
    /**
     * @brief 错误信息
     * 
     */
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
/**
 * @brief 添加打印配置 request \see RespAddConfig
 * 
 */
struct RequestAddConfig {
    static constexpr inline std::string_view method{"add_config"};
    std::optional<int>                       uid;
    PrinterConfig                            data{};
};
/**
 * @brief 添加打印配置 response \see RespError
 * 
 */
struct RespAddConfig : RespBase {
    /**
     * @brief 打印配置id,由sqlite 生成
     * 
     */
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
/**
 * @brief 更新打印配置 request \see RespUpdateConfig
 * 
 */
struct RequestUpdateConfig {
    static constexpr inline std::string_view method{"update_config"};
    std::optional<int>                       uid;
    PrinterConfig                            data{};
};
/**
 * @brief 更新打印配置 response \see RespError
 * 
 */
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
/**
 * @brief 删除打印配置 request \see RespDelConfig
 * 
 */
struct RequestDelConfig {
    static constexpr inline std::string_view method{"del_config"};
    std::optional<int>                       uid;
    /**
     * @brief 打印配置id 
     * 
     */
    int                                      data{};
};
/**
 * @brief 删除打印配置 response \see RespError
 * 
 */
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
/**
 * @brief 获取所有打印配置 request \see RespGetAllConfigs
 * 
 */
struct RequestGetAllConfigs {
    static constexpr inline std::string_view method{"get_all_configs"};
    std::optional<int>                       uid;
};
/**
 * @brief 获取所有打印配置 response \see RespError
 * 
 */
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
/**
 * @brief 倒序打印页面获取 request \see RespGetPagesDesc
 * 
 */
struct RequestGetPagesDesc {
    static constexpr inline std::string_view method{"get_pages_desc"};
    std::optional<int>                       uid;
    int                                      page_index{1};
    int                                      page_size{10};
};
/**
 * @brief 倒序打印页面获取 response \see RespError
 * 
 */
struct RespGetPagesDesc : RespBase {
    std::deque<PrinterConfig> data{};

    /**
     * @brief 总数目
     * 
     */
    int count{0};
    static QJsonObject        toJsonObject(std::optional<int>             uid,
                                           const std::deque<PrintedPage>& configs,int count) {
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
        obj["count"] = count;
        return obj;
    }
};
/**
 * @brief 获取 websocket 服务器端口号 request \see RespGetWebsocketServerPort
 * 
 */
struct RequestGetWebsocketServerPort {
    static constexpr inline std::string_view method{"get_websocket_server_port"};
    std::optional<int>                       uid;
};
/**
 * @brief 获取 websocket 服务器端口号 response
 * 
 */
struct RespGetWebsocketServerPort : RespBase {
    /**
     * @brief websocket 端口号
     * 
     */
    QString            data{""};
    static QJsonObject toJsonObject(std::optional<int> uid, const QString& port) {
        QJsonObject obj;
        obj["is_success"] = true;
        if (uid.has_value()) {
            obj["uid"] = uid.value();
        }
        obj["data"] = port;
        return obj;
    }
};
/**
 * @brief 获取打印机信息 request \see RespGetPrintersInfo
 * 
 */
struct RequestGetPrintersInfo {
    static constexpr inline std::string_view method{"get_printers_info"};
    std::optional<int>                       uid;
};
struct PrinterInfo {
    QString        printer_name{""};
    QList<QString> supported_paper_names{};
};
/**
 * @brief 获取打印机信息 response \see RespError
 * 
 */
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

/**
 * @brief 打印页面 request \see RespPrintPage
 * 
 */
struct RequestPrintPage {
    static constexpr inline std::string_view method{"print_page"};
    std::optional<int>                       uid;
    PrintedPage                              data{};
};
/**
 * @brief 打印页面 response \see RespError
 * 
 */
struct RespPrintPage : RespBase {
    /**
     * @brief 输出的文件路径,不包含后缀,(.pdf 或.png)
     * 
     */
    QString file_path{""};
    /**
     * @brief sqlite中存储的对应数据的id
     * 
     */
    int     page_id{0};

    static QJsonObject toJsonObject(std::optional<int> uid, const QString& file_path, int page_id) {
        QJsonObject obj;
        obj["is_success"] = true;
        if (uid.has_value()) {
            obj["uid"] = uid.value();
        }
        obj["file_path"] = file_path;
        obj["page_id"] = page_id;
        return obj;
    }
};
