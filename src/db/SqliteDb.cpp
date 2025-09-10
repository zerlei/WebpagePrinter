#include "SqliteDb.h"
#include "../InitConfig.h"
#include "../excep/FatalError.h"
#include "../excep/SqliteOpError.h"
#include "../log/log.h"
#include "../model/ModelsSql.h"
#include <QFile>
#include <QSqlError>
#include <QString>
#include <algorithm>
#include <array>
#include <cstddef>
#include <deque>
#include <qcontainerfwd.h>
#include <qdebug.h>
#include <qlogging.h>
#include <tuple>
#define LogAddThrow(arg)                                                                           \
    qCritical(APPLOG) << (arg);                                                                    \
    throw SqliteOpError((arg))

SqliteDb::SqliteDb() {
    QString db_path = QString(InitConfig::base_dir) + "data.db";
    if (QFile::exists(db_path)) {
        initDb();
        if (!checkValid()) {
            throw FatalError("Check Table struct error!");
        }
    } else {
        createDb();
    }
}
SqliteDb& SqliteDb::instance() {
    static SqliteDb db;
    return db;
}
void SqliteDb::createDb() {
    initDb();

    constexpr auto page_table_str   = create_page_table_str.data;
    constexpr auto config_table_str = create_config_table_str.data;

    if (!query->exec(page_table_str) || !query->exec(config_table_str)) {
        throw FatalError(QString("Create Page Error: ") + query->lastError().text());
    }
    if (!checkValid()) {
        throw FatalError("Check Table struct error!");
    }
}
void SqliteDb::initDb() {
    QString db_path = QString(InitConfig::base_dir) + "data.db";
    db              = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(db_path);
    if (!db.open()) {
        throw FatalError("Failed to open sqlite database");
    };
    query = std::make_unique<QSqlQuery>(db);
}

template <size_t N>
bool judgeFieldEqual(const std::deque<std::pair<QString, QString>>& ptt,
                     std::array<PrinterConfigToTable, N>            rv) {

    if (ptt.size() != N) {
        return false;
    }
    for (auto& [field_name, field_data_type] : ptt) {
        auto fv = std::find_if(rv.cbegin(), rv.cend(), [&field_name, &field_data_type](auto& e) {
            if (field_name.toStdString() == std::string(e.field_name) &&
                field_data_type.toStdString() == std::string(e.field_data_type)) {
                return true;
            }
            return false;
        });
        if (fv == rv.cend()) {
            return false;
        }
    }
    return true;
}

bool SqliteDb::checkValid() {

    // check table
    auto table_result    = std::deque<QString>();
    auto check_table_sql = QString(R"(
    SELECT name 
    FROM sqlite_master 
    WHERE type = 'table' 
          AND name NOT LIKE 'sqlite_%';)");
    if (query->exec(check_table_sql)) {
        while (query->next()) {
            table_result.emplace_back(query.get()->value(0).toString());
        }

        for (auto& tn : tables_name) {

            auto fr = std::find_if(table_result.cbegin(), table_result.cend(), [&tn](auto& e) {
                if (tn == QString(e)) {
                    return true;
                }
                return false;
            });
            if (fr == table_result.cend()) {
                return false;
            }
        }

    } else {
        return false;
    }

    // check table structure
    size_t i = 0;
    for (; i < tables_name.size(); i++) {
        auto& tn                 = tables_name[i];
        auto  ptt                = std::deque<std::pair<QString, QString>>();
        auto  check_table_fields = QString("PRAGMA table_info(%1)").arg(tn);
        if (query->exec(check_table_fields)) {
            while (query->next()) {
                ptt.emplace_back(query->value("name").toString(), query->value("type").toString());
            }
            if (i == 0) {
                if (!judgeFieldEqual(ptt, printed_page_table_helper)) {
                    return false;
                }
            } else if (i == 1) {
                if (!judgeFieldEqual(ptt, printer_config_table_helper)) {
                    return false;
                }
            }
        } else {
            return false;
        }
    }
    return true;
}
void SqliteDb::addConfig(PrinterConfig& config) const {

    QString insert_command = "INSERT into printer_config (";
    QString after_command  = " VALUES (";
    for (size_t i = 1; i < printer_config_table_helper.size(); i++) {
        if (i < printer_config_table_helper.size() - 1) {
            insert_command += printer_config_table_helper[i].field_name + ", ";
            if (printer_config_table_helper[i].field_data_type == "INTEGER") {
                after_command += QString("%%1,").arg(i);
            } else {
                after_command += QString("'%%1',").arg(i);
            }
        } else {

            insert_command += printer_config_table_helper[i].field_name + ") ";
            if (printer_config_table_helper[i].field_data_type == "INTEGER") {
                after_command += QString("%%1)").arg(i);
            } else {
                after_command += QString("'%%1')").arg(i);
            }
        }
    }
    insert_command += after_command;

    insert_command = insert_command.arg(config.name)
                         .arg(config.is_save_png)
                         .arg(config.is_to_printer)
                         .arg(config.width_mm)
                         .arg(config.height_mm)
                         .arg(config.top_margin)
                         .arg(config.bottom_margin)
                         .arg(config.left_margin)
                         .arg(config.right_margin)
                         .arg(config.printer_name)
                         .arg(config.is_use_printer_default_config)
                         .arg(config.printer_paper_name)
                         .arg(config.printer_orientation)
                         .arg(config.process_at_end)
                         .arg(config.process_argument_at_end);

    if (query->exec(insert_command)) {
        config.id = query->lastInsertId().toInt();
    } else {
        LogAddThrow(QString("Create Config Error: ") + query->lastError().text());
    }
}

void SqliteDb::updateConfig(const PrinterConfig& printer_config) const {

    QString update_command = QString("UPDATE printer_config SET ");
    QString fields_str     = QString(update_config_table_str.data)
                             .arg(printer_config.name)
                             .arg(printer_config.is_save_png)
                             .arg(printer_config.is_to_printer)
                             .arg(printer_config.width_mm)
                             .arg(printer_config.height_mm)
                             .arg(printer_config.top_margin)
                             .arg(printer_config.bottom_margin)
                             .arg(printer_config.left_margin)
                             .arg(printer_config.right_margin)
                             .arg(printer_config.printer_name)
                             .arg(printer_config.is_use_printer_default_config)
                             .arg(printer_config.printer_paper_name)
                             .arg(printer_config.printer_orientation)
                             .arg(printer_config.process_at_end)
                             .arg(printer_config.process_argument_at_end);
    QString after_command = " WHERE id=" + QString::number(printer_config.id);
    update_command += fields_str + after_command;
    if (!query->exec(update_command)) {
        LogAddThrow(QString("Update Config Error: ") + query->lastError().text());
    }
}
void SqliteDb::delConfig(int id) const {

    QString del_cmd = "DELETE FROM printer_config WHERE id = " + QString::number(id);
    if (!query->exec(del_cmd)) {
        LogAddThrow(QString("Del Config Error: ") + query->lastError().text());
    }
};
PrinterConfig SqliteDb::getConfigById(int id) const {
    PrinterConfig printer_config;
    QString       query_command = "SELECT * FROM printer_config WHERE id = " + QString::number(id);
    if (query->exec(query_command)) {
        if (query->next()) {
            printer_config.name          = query->value("name").toString();
            printer_config.is_save_png   = query->value("is_save_png").toInt();
            printer_config.is_to_printer = query->value("is_to_printer").toInt();
            printer_config.width_mm      = query->value("width_mm").toInt();
            printer_config.height_mm     = query->value("height_mm").toInt();
            printer_config.top_margin    = query->value("top_margin").toInt();
            printer_config.bottom_margin = query->value("bottom_margin").toInt();
            printer_config.left_margin   = query->value("left_margin").toInt();
            printer_config.right_margin  = query->value("right_margin").toInt();
            printer_config.printer_name  = query->value("printer_name").toString();
            printer_config.is_use_printer_default_config =
                query->value("is_use_printer_default_config").toInt();
            printer_config.printer_paper_name  = query->value("printer_paper_name").toString();
            printer_config.printer_orientation = query->value("printer_orientation").toString();
            printer_config.process_at_end      = query->value("process_at_end").toString();
            printer_config.process_argument_at_end =
                query->value("process_argument_at_end").toString();
            printer_config.id = query->value("id").toInt();
        } else {
            LogAddThrow(QString("can't find this config (id: %1)").arg(id));
        }
    } else {
        LogAddThrow(QString("Select Config Error: ") + query->lastError().text());
    }
    return printer_config;
};
std::deque<PrinterConfig> SqliteDb::getAllConfigs() const {
    std::deque<PrinterConfig> configs;
    if (query->exec("SELECT * FROM printer_config")) {
        while (query->next()) {
            configs.emplace_back();
            auto& config         = configs.back();
            config.name          = query->value("name").toString();
            config.is_save_png   = query->value("is_save_png").toInt();
            config.is_to_printer = query->value("is_to_printer").toInt();
            config.width_mm      = query->value("width_mm").toInt();
            config.height_mm     = query->value("height_mm").toInt();
            config.top_margin    = query->value("top_margin").toInt();
            config.bottom_margin = query->value("bottom_margin").toInt();
            config.left_margin   = query->value("left_margin").toInt();
            config.right_margin  = query->value("right_margin").toInt();
            config.printer_name  = query->value("printer_name").toString();
            config.is_use_printer_default_config =
                query->value("is_use_printer_default_config").toInt();
            config.printer_paper_name      = query->value("printer_paper_name").toString();
            config.printer_orientation     = query->value("printer_orientation").toString();
            config.process_at_end          = query->value("process_at_end").toString();
            config.process_argument_at_end = query->value("process_argument_at_end").toString();
            config.id                      = query->value("id").toInt();
        }
    } else {
        LogAddThrow(QString("Select Config Error: ") + query->lastError().text());
    }
    return configs;
}

void SqliteDb::addPage(PrintedPage& page) const {

    QString insert_command = "INSERT into printed_page (";
    QString after_command  = " VALUES (";
    for (size_t i = 1; i < printed_page_table_helper.size(); i++) {
        if (i < printed_page_table_helper.size() - 1) {
            insert_command += printed_page_table_helper[i].field_name + ", ";
            if (printed_page_table_helper[i].field_data_type == "INTEGER") {
                after_command += QString("%%1,").arg(i);
            } else {
                after_command += QString("'%%1',").arg(i);
            }
        } else {
            insert_command += printed_page_table_helper[i].field_name + ") ";
            if (printed_page_table_helper[i].field_data_type == "INTEGER") {
                after_command += QString("%%1)").arg(i);
            } else {
                after_command += QString("'%%1')").arg(i);
            }
        }
    }
    insert_command += after_command;
    insert_command = insert_command.arg(page.config_id)
                         .arg(page.page_loaded_or_js_request)
                         .arg(page.time)
                         .arg(page.status)
                         .arg(page.from_ip)
                         .arg(page.page_file_path)
                         .arg(page.page_url)
                         .arg(page.end_cmd_exec_status)
                         .arg(page.end_cmd_exec_message)
                         .arg(page.error_message);

    qDebug() << insert_command;
    if (query->exec(insert_command)) {
        page.id = query->lastInsertId().toInt();
    } else {
        LogAddThrow(QString("Create Page Error: ") + query->lastError().text());
    }
}
void SqliteDb::updatePage(const PrintedPage& page) const {
    QString update_command = QString("UPDATE printed_page SET ");
    QString fields_str     = QString(update_page_table_str.data)
                             .arg(page.config_id)
                             .arg(page.page_loaded_or_js_request)
                             .arg(page.time)
                             .arg(page.status)
                             .arg(page.from_ip)
                             .arg(page.page_file_path)
                             .arg(page.page_url)
                             .arg(page.end_cmd_exec_status)
                             .arg(page.end_cmd_exec_message)
                             .arg(page.error_message);
    QString after_command = " WHERE id=" + QString::number(page.id);
    update_command += fields_str + after_command;
    if (!query->exec(update_command)) {
        LogAddThrow(QString("Update Page Error: ") + query->lastError().text());
    }
}
std::tuple<std::deque<PrintedPage>, int> SqliteDb::getPagesDesc(int page_index, int page_size) const {

    std::deque<PrintedPage> pages;
    if (query->exec(QString("SELECT * FROM printed_page order by id desc limit %1 offset %2")
                        .arg(page_size)
                        .arg(page_size * (page_index - 1)))) {
        while (query->next()) {
            pages.emplace_back();
            auto& page                     = pages.back();
            page.id                        = query->value("id").toInt();
            page.config_id                 = query->value("config_id").toInt();
            page.page_loaded_or_js_request = query->value("page_loaded_or_js_request").toString();
            page.time                      = query->value("time").toString();
            page.status                    = query->value("status").toString();
            page.from_ip                   = query->value("from_ip").toString();
            page.page_file_path            = query->value("page_file_path").toString();
            page.page_url                  = query->value("page_url").toString();
            page.end_cmd_exec_message      = query->value("end_cmd_exec_message").toString();
            page.error_message             = query->value("error_message").toString();
        }
    } else {
        LogAddThrow(QString("Select Page Error: ") + query->lastError().text());
    }

    int count{0};
    if (query->exec("SELECT count(1) from printed_page")) {
        while (query->next()) {
            count = query->value(0).toInt();
        }
    } else {
        LogAddThrow(QString("Select Page Count Error: ") + query->lastError().text());
    }
    return std::make_tuple(pages, count);
}