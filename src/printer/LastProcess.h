#pragma once
#include "../model/WebInterface.h"
#include "../db/SqliteDb.h"
#include "DataPack.h"
#include <QProcess>
class LastProcess {
    static constexpr STEP step = STEP::LAST_PROCESS;

  public:
    void work(PrinterDataPack& data_pack) {
        if (data_pack.config.process_at_end.isEmpty()) {
            data_pack.setRespValue(RespPrintPage::toJsonObject(
                data_pack.uid, data_pack.page.page_file_path, data_pack.page.id));
        } else {

            QProcess process;

            auto process_argumnet = data_pack.config.process_argument_at_end.replace(
                "%PAGE_FILE_PATH%", data_pack.page.page_file_path);

            process.start(data_pack.config.process_at_end, process_argumnet.split(" "));
            process.waitForFinished();
            data_pack.page.end_cmd_exec_message = process.readAllStandardOutput();
            data_pack.page.end_cmd_exec_status  = process.exitCode();
            if (data_pack.page.end_cmd_exec_status != 0) {
                data_pack.page.error_message = process.readAllStandardError();
                data_pack.setRespValue(
                    RespError::toJsonObject(data_pack.uid, data_pack.page.error_message));
            } else {
                data_pack.page.status = step_str[step];
                SqliteDb::instance().updatePage(data_pack.page);
                data_pack.setRespValue(RespPrintPage::toJsonObject(
                    data_pack.uid, data_pack.page.page_file_path, data_pack.page.id));
            }
        }
    }
};
