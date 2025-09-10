#pragma once
#include "../db/SqliteDb.h"
#include "../excep/CanExceptionCallback.h"
#include "../excep/PrintWorkFlowError.h"
#include "../excep/SqliteOpError.h"
#include "../model/WebInterface.h"
#include "DataPack.h"
#include "WebRender.h"
template <typename T>
class PageRender {
    T         next;
    WebRender render;

    static constexpr STEP step = STEP::PAGE_RENDER;

  public:
    void work(PrinterDataPack& data_pack) {
        qDebug()<<step_str[step];
        CanExceptionCallback callback(
            [this, &data_pack]() {
                try {
                    data_pack.page.status = step_str[step];
                    SqliteDb::instance().updatePage(data_pack.page);
                    next.work(data_pack);
                } catch (const SqliteOpError& e) {
                    data_pack.setRespValue(RespError::toJsonObject(data_pack.uid, e.what()));
                }
            },
            [this, &data_pack](const PrintWorkFlowError&) {
                SqliteDb::instance().updatePage(data_pack.page);
                data_pack.setRespValue(
                    RespError::toJsonObject(data_pack.uid, data_pack.page.error_message));
            });
        render.work(&data_pack, callback);
    }
};