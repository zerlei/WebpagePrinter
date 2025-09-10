#pragma once
#include "../db/SqliteDb.h"
#include "../excep/JsonParseError.h"
#include "../excep/SqliteOpError.h"
#include "../model/ModelsJson.h"
#include "../model/PrintedPage.h"
#include "../model/PrinterConfig.h"
#include "../model/WebInterface.h"
#include <array>
#include <functional>
#include <optional>
#include <qjsonobject.h>
#include <tuple>
enum STEP { GET_CONFIG = 0, PAGE_RENDER = 1, RENDER_PNG = 2, PRINTE = 3, LAST_PROCESS = 4 };
constexpr std::array<char const*, 5> step_str = {"GET_CONFIG", "PAGE_RENDER", "RENDER_PNG",
                                                 "PRINTE",
                                                 "LAST_PROCESS"};
struct PrinterDataPack {

    PrintedPage           page;
    PrinterConfig         config;
    std::optional<int>    uid;
    std::function<void()> monitor_promise;
    PrinterDataPack() = delete;
    explicit PrinterDataPack(std::function<void()> f)
        : monitor_promise(f) {};
    void setData(std::tuple<QJsonObject, std::move_only_function<void(QJsonObject)>,
                            std::optional<int>>& t) {
        resp = std::move(std::get<1>(t));
        uid  = std::get<2>(t);
        try {
            page      = fromPrintedPageJson("", std::get<0>(t));
            page.time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            SqliteDb::instance().addPage(page);
        } catch (const JsonParseError& e) {
            setRespValue(RespError::toJsonObject(uid, e.what()));
        } catch (const SqliteOpError& e) {
            setRespValue(RespError::toJsonObject(uid, e.what()));
        }
    }
    void setRespValue(QJsonObject obj) {
        resp(obj);
        monitor_promise();
    }

  private:
    std::move_only_function<void(QJsonObject)> resp;
};