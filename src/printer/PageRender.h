#pragma once
#include "../db/SqliteDb.h"
#include "../model/WebInterface.h"
#include "DataPack.h"
#include "WebRender.h"
#include <future>
template <typename T>
class PageRender {
    T         next;
    WebRender render;

    static constexpr STEP step = STEP::PAGE_RENDER;

  public:
    void work(PrinterDataPack& data_pack) {
        try {

            std::promise<void> p;
            auto               f = p.get_future();
            render.work(&data_pack, std::move(p));
            f.wait();
            data_pack.page.status = step_str[step];
            SqliteDb::instance().updatePage(data_pack.page);
            next.work(data_pack);
        } catch (int v) {
            data_pack.setPromiseValue(
                RespError::toJsonObject(data_pack.uid, data_pack.page.error_message));
        }
    }
};