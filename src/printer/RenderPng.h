#pragma once
#include "../exception/PrintWorkFlowError.h"
#include "../model/WebInterface.h"
#include "DataPack.h"
#include "Printer.h"
#include <future>
template <typename T>
class RenderPng {
    T       next;
  public:
    void work(PrinterDataPack& data_pack) {
        try {
            if (data_pack.config.is_save_png == 1) {
                next.printer.setDataPack(&data_pack);
                std::promise<void> promise;
                auto               future = promise.get_future();
               next.printer.renderPng(std::move(promise));
                future.get();
            }
            next.work(data_pack);
        } catch (const PrintWorkFlowError&) {
            data_pack.setPromiseValue(
                RespError::toJsonObject(data_pack.uid, data_pack.page.error_message));
        }
    }
};
