#pragma once
#include "../excep/CanExceptionCallback.h"
#include "../excep/PrintWorkFlowError.h"
#include "../model/WebInterface.h"
#include "DataPack.h"
template <typename T>
class RenderPng {
    static constexpr STEP step = STEP::RENDER_PNG;
    T                     next;

  public:
    void work(PrinterDataPack& data_pack) {
        qDebug()<<step_str[step];
        if (data_pack.config.is_save_png == 1) {
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
            next.printer.setDataPack(&data_pack);
            next.printer.renderPng(callback);
        } else {
            next.work(data_pack);
        }
    }
};
