#pragma once
#include "../exception/PrintWorkFlowError.h"
#include "../model/WebInterface.h"
#include "DataPack.h"
#include "Printer.h"
#include <future>
template <typename T>
class ToPrinter {
    T next;

  public:
    Printer printer;

  public:
    void work(PrinterDataPack& data_pack) {
        try {
            if (data_pack.config.is_to_printer == 1) {
                printer.setDataPack(&data_pack);
                std::promise<void> promise;
                auto               future = promise.get_future();
                printer.toPrinter(std::move(promise));
                future.get();
            }

        } catch (const PrintWorkFlowError& e) {
            data_pack.setPromiseValue(
                RespError::toJsonObject(data_pack.uid, data_pack.page.error_message));
        }
    }
};
