#pragma once
#include "../db/SqliteDb.h"
#include "../model/WebInterface.h"
#include "DataPack.h"
#include <QProcess>
class LastProcess {
    static constexpr STEP step = STEP::LAST_PROCESS;

  public:
    void work(PrinterDataPack& data_pack) {
        qDebug() << step_str[step];
        if (data_pack.config.process_at_end.isEmpty()) {
            data_pack.setRespValue(RespPrintPage::toJsonObject(
                data_pack.uid, data_pack.page.page_file_path, data_pack.page.id));
        } else {

            QProcess process;
            process.setEnvironment(QProcess::systemEnvironment());
            auto process_argumnet = data_pack.config.process_argument_at_end.replace(
                "%PAGE_FILE_PATH%", data_pack.page.page_file_path);

            process.start(data_pack.config.process_at_end, process_argumnet.split(" "));
            bool l = process.waitForFinished();
            if (!l) {
                qDebug() << process.error();
                qDebug() << process.errorString();
                qDebug() << "in nix bundle, host program not work!";
            }
            data_pack.page.end_cmd_exec_message = process.readAllStandardOutput();
            data_pack.page.end_cmd_exec_status  = process.exitCode();
            if (process.exitStatus() != QProcess::NormalExit) {
                data_pack.page.error_message = process.readAllStandardError();
                SqliteDb::instance().updatePage(data_pack.page);
                data_pack.setRespValue(
                    RespError::toJsonObject(data_pack.uid, data_pack.page.error_message));
            } else {
                // qDebug()<<step_str[step]<<data_pack.page.end_cmd_exec_message;
                data_pack.page.status = step_str[step];
                SqliteDb::instance().updatePage(data_pack.page);
                data_pack.setRespValue(RespPrintPage::toJsonObject(
                    data_pack.uid, data_pack.page.page_file_path, data_pack.page.id));
            }
        }
    }
};
