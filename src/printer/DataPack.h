#pragma once
#include "../model/ModelsJson.h"
#include "../model/PrintedPage.h"
#include "../model/PrinterConfig.h"
#include <array>
#include <functional>
#include <future>
#include <optional>
#include <qjsonobject.h>
#include <tuple>
enum STEP { GET_CONFIG = 0, PAGE_RENDER = 1, RENDER_PNG = 2, PRINTE = 3, LAST_CMD = 4 };
constexpr std::array<char const*, 4> step_str = {"GET_CONFIG", "PAGE_RENDER", "RENDER_PNG",
                                                 "PRINTE"
                                                 "LAST_CMD"};
struct PrinterDataPack {

    PrintedPage           page;
    PrinterConfig         config;
    std::optional<int>    uid;
    std::function<void()> monitor_promise;
    PrinterDataPack() = delete;
    explicit PrinterDataPack(std::function<void()> f)
        : monitor_promise(f) {};
    void updateData(std::tuple<QJsonObject, std::promise<QJsonObject>>& t) {
        p    = std::move(std::get<1>(t));
        page = fromPrintedPageJson("", std::get<0>(t));
    }
    void setPromiseValue(QJsonObject obj) {
        p.set_value(obj);
        monitor_promise();
    }

  private:
    std::promise<QJsonObject> p;
};