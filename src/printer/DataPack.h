#pragma once
#include <array>
#include <optional>
#pragma once
#include "../model/PrintedPage.h"
#include "../model/PrinterConfig.h"
#include <future>
#include <qjsonobject.h>
enum STEP { GET_CONFIG = 0, LOADRENDER_PDF = 1, RENDER_PNG = 2, LAST_CMD = 3 };
constexpr std::array<char const*, 4> step_str = {"GET_CONFIG", "LOADRENDER_PDF", "RENDER_PNG",
                                                 "LAST_CMD"};
struct PrinterDataPack {

    PrintedPage               page;
    PrinterConfig             config;
    std::optional<int>        uid;
    std::promise<QJsonObject> p;
};