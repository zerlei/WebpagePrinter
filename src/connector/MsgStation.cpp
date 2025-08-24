#include "MsgStation.h"
#include "../InitConfig.h"
#include "../db/SqliteDb.h"
#include "../exception/JsonParseError.h"
#include "../model/ModelsJson.h"
#include "../model/WebInterface.h"
#include "HttpServer.h"
#include <memory>
#include <qjsonobject.h>
MsgStation::MsgStation() {
    initMsgHandler();
    auto [http_server_ip, http_server_port] = InitConfig::instance().getHttpServerIpPort();
    auto [websocket_local_ip, websocket_local_port] =
        InitConfig::instance().getLocalWebsocketServerIpPort();
    auto websocket_remote_url = InitConfig::instance().getRemoteWebsocketServerUrl();
    http_server = std::make_unique<HttpServer>(message_handler, http_server_ip, http_server_port);
    websocket_client = std::make_unique<WebsocketClient>(websocket_remote_url, message_handler);
    websocket_server = std::make_unique<WebsocketServer>(websocket_local_ip, websocket_local_port,
                                                         message_handler);
}
void MsgStation::initMsgHandler() {

    message_handler = [this](const QString& msg, const QString& ip, const QString& from,
                             std::promise<QJsonObject> p) {
        // uid websocket 的消息request 和 resp 不限制对应关系,uid 保证了他们的联系,这个由Request
        // 保证 也就是 uid 相同 的 request 和 response 对应
        std::optional<int> uid;
        try {

            QJsonParseError parse_error;
            QJsonDocument   doc = QJsonDocument::fromJson(msg.toUtf8(), &parse_error);
            if (parse_error.error != QJsonParseError::NoError) {
                throw JsonParseError(msg, "Parse error:" + parse_error.errorString());
            }
            if (!doc.isNull() && doc.isObject()) {
                QJsonObject obj = doc.object();
                if (obj["uid"].isDouble()) {
                    uid = obj["uid"].toInt();
                }
                if (obj["method"].isString()) {
                    auto methond = obj["method"].toString();
                    switch (hash(methond.toStdString())) {
                    case hash(RequestAddConfig::method): {
                        if (obj["data"].isObject()) {
                            auto config = fromPrinterConfigJson(msg, obj["data"].toObject());
                            SqliteDb::instance().addConfig(config);
                            p.set_value(RespAddConfig::toJsonObject(uid, config.id));
                        } else {
                            throw JsonParseError(msg, "Parse error: data is not a json object");
                        }
                        break;
                    }
                    case hash(RequestUpdateConfig::method): {
                        if (obj["data"].isObject()) {
                            auto config = fromPrinterConfigJson(msg, obj["data"].toObject());
                            SqliteDb::instance().updateConfig(config);
                            p.set_value(RespUpdateConfig::toJsonObject(uid));
                        } else {
                            throw JsonParseError(msg, "Parse error: data is not a json object");
                        }
                        break;
                    }
                    case hash(RequestDelConfig::method): {
                        if (obj["data"].isDouble()) {
                            auto config_id = obj["data"].toInt();
                            SqliteDb::instance().delConfig(config_id);
                            p.set_value(RespDelConfig::toJsonObject(uid));
                        } else {
                            throw JsonParseError(msg, "Parse error: data is not a json object");
                        }
                        break;
                    }
                    case hash(RequestGetAllConfigs::method): {
                        p.set_value(RespGetAllConfigs::toJsonObject(
                            uid, SqliteDb::instance().getAllConfigs()));
                        break;
                    }
                    case hash(RequestGetPagesDesc::method): {
                        if (obj["page_index"].isDouble() && obj["page_size"].isDouble()) {
                            auto                    page_index = obj["page_index"].toInt();
                            auto                    page_size  = obj["page_size"].toInt();
                            std::deque<PrintedPage> pages =
                                SqliteDb::instance().getPagesDesc(page_index, page_size);
                            p.set_value(RespGetPagesDesc::toJsonObject(uid, pages));

                        } else {
                            throw JsonParseError(msg, "Parse error: data is not a json object");
                        }
                        break;
                    }
                    case hash(RequestPrintPage::method): {

                        if (obj["data"].isObject()) {
                            auto page      = obj["data"].toObject();
                            obj["from_ip"] = from + "--" + ip;
                            printer_work_flow.addWorkQueue(page, std::move(p));
                        } else {
                            throw JsonParseError(msg, "Parse error: data is not a json object");
                        }
                        break;
                    }

                    default: {
                        throw JsonParseError(msg, "Parse error: no such method:" + methond);
                    }
                    }

                } else {
                    throw JsonParseError(msg, "Parse error: id is not a number");
                }
            } else {
                throw JsonParseError(msg, "Parse error: is not an Json object");
            }
        } catch (const JsonParseError& ex) {
            p.set_value(RespError::toJsonObject(uid, ex.what()));
        }
    };
}
