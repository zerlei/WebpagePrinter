#include "../src/InitConfig.h"
#include "../src/printer/GetConfig.h"
#include "../src/printer/LastProcess.h"
#include "../src/printer/PageRender.h"
#include "../src/printer/RenderPng.h"
#include "../src/printer/ToPrinter.h"
#include "../src/printer/WorkFlow.h"
#include <QEventLoop>
#include <QFile>
#include <QSignalSpy>
#include <QString>
#include <QTest>
#include <QTextStream>
#include <algorithm>
#include <functional>
#include <optional>
#include <qloggingcategory.h>
#include <qnumeric.h>
#include <qtestcase.h>
class TestPrint : public QObject {
    Q_OBJECT
      private:
        PrinterWorkFlow<GetConfig<PageRender<RenderPng<ToPrinter<LastProcess>>>>>*
            printer_work_flow;

      private:
        PrinterConfig newConfig() {
            PrinterConfig config;
            config.id                            = 0;
            config.name                          = "t";
            config.is_save_png                   = 1;
            config.is_to_printer                 = 0;
            config.width_mm                      = 1920;
            config.height_mm                     = 1080;
            config.left_margin                   = 0;
            config.right_margin                  = 0;
            config.top_margin                    = 0;
            config.bottom_margin                 = 0;
            config.printer_name                  = "";
            config.is_use_printer_default_config = 0;
            config.printer_paper_name            = "";
            config.printer_orientation           = "Landscape";
            config.process_at_end                = "echo";
            config.process_argument_at_end       = "%PAGE_FILE_PATH%";
            return config;
        }
        QJsonObject newPage() {
            QJsonObject page;
            page["page_loaded_or_js_request"] = "PAGE_LOADED";
            page["page_url"]                  = "https://www.baidu.com";
            return page;
        }
      private slots:

        void initTestCase() {
            QDir dir(InitConfig::base_dir);
            if (dir.exists()) {
                dir.removeRecursively();
            }
            InitConfig::instance();
            SqliteDb::instance();
            printer_work_flow =
                new PrinterWorkFlow<GetConfig<PageRender<RenderPng<ToPrinter<LastProcess>>>>>();
        }
        void cleanupTestCase() {
            if (printer_work_flow != nullptr) {
                delete printer_work_flow;
                printer_work_flow = nullptr;
            }
            QDir dir(InitConfig::base_dir);
            if (dir.exists()) {
                if (!dir.removeRecursively()) {
                    qWarning() << "Failed to remove directory:" << InitConfig::base_dir;
                }
            }
        }
        void testJsRequest() {

            auto config = newConfig();
            SqliteDb::instance().addConfig(config);
            auto page                         = newPage();
            page["page_loaded_or_js_request"] = "JS_REQUEST";

            auto html_path = QString("file://") + QCoreApplication::applicationDirPath() +
                             "/../../tests/JSREQUEST.html";
            page["page_url"]  = html_path;
            page["config_id"] = config.id;
            QEventLoop loop;

            QJsonObject re;

            std::move_only_function<void(QJsonObject)> f = [&loop, &re](QJsonObject result) {
                re = result;
                loop.quit();
                qDebug() << "Loop quit";
            };
            std::optional<int> uid = 0;
            // QSignalSpy spy(printer_work_flow->first_step.next.)
            printer_work_flow->addWorkQueue(page, std::move(f), uid);

            QTimer::singleShot(1000 * 60 * 5, &loop, &QEventLoop::quit);
            loop.exec();
            qDebug() << QJsonDocument(re).toJson();
            QVERIFY(re["is_success"].toBool() == true);
            QVERIFY(QFile::exists(re["file_path"].toString() + ".pdf"));
            QVERIFY(QFile::exists(re["file_path"].toString() + ".png"));
            auto [pages,_]     = SqliteDb::instance().getPagesDesc(0, 100);
            auto c_pageid  = re["page_id"].toInt();
            auto db_c_page = std::find_if(pages.begin(), pages.end(), [c_pageid](const auto& page) {
                return page.id == c_pageid;
            });
            QVERIFY(db_c_page != pages.end());
            QVERIFY(db_c_page->status == "LAST_PROCESS");
            QVERIFY(db_c_page->end_cmd_exec_status == 0);
            QVERIFY(db_c_page->end_cmd_exec_message.replace("\n", "") ==
                    re["file_path"].toString());
        }
        void testPageLoaded() {
            return;
            auto config = newConfig();
            SqliteDb::instance().addConfig(config);
            auto page         = newPage();
            page["config_id"] = config.id;
            QEventLoop loop;

            QJsonObject re;

            std::move_only_function<void(QJsonObject)> f = [&loop, &re](QJsonObject result) {
                re = result;
                loop.quit();
                qDebug() << "Loop quit";
            };
            std::optional<int> uid = 0;
            // QSignalSpy spy(printer_work_flow->first_step.next.)
            printer_work_flow->addWorkQueue(page, std::move(f), uid);

            QTimer::singleShot(1000 * 60 * 5, &loop, &QEventLoop::quit);
            loop.exec();
            qDebug() << QJsonDocument(re).toJson();
            QVERIFY(re["is_success"].toBool() == true);
            QVERIFY(QFile::exists(re["file_path"].toString() + ".pdf"));
            QVERIFY(QFile::exists(re["file_path"].toString() + ".png"));
            auto [pages,_]     = SqliteDb::instance().getPagesDesc(0, 100);
            auto c_pageid  = re["page_id"].toInt();
            auto db_c_page = std::find_if(pages.begin(), pages.end(), [c_pageid](const auto& page) {
                return page.id == c_pageid;
            });
            QVERIFY(db_c_page != pages.end());
            QVERIFY(db_c_page->status == "LAST_PROCESS");
            QVERIFY(db_c_page->end_cmd_exec_status == 0);
            QVERIFY(db_c_page->end_cmd_exec_message.replace("\n", "") ==
                    re["file_path"].toString());
        }
#ifdef PrintName
        void testPrinterDefaultConfig() {
            auto config = newConfig();
            config.printer_name = PrintName;
            config.is_to_printer = 1;
            config.is_use_printer_default_config = 1;
            // config.printer_paper_name = PaperName;
            SqliteDb::instance().addConfig(config);
            auto page         = newPage();
            page["config_id"] = config.id;
            QEventLoop loop;

            QJsonObject re;

            std::move_only_function<void(QJsonObject)> f = [&loop, &re](QJsonObject result) {
                re = result;
                loop.quit();
                qDebug() << "Loop quit";
            };
            std::optional<int> uid = 0;
            // QSignalSpy spy(printer_work_flow->first_step.next.)
            printer_work_flow->addWorkQueue(page, std::move(f), uid);

            QTimer::singleShot(1000 * 60 * 5, &loop, &QEventLoop::quit);
            loop.exec();
            qDebug() << QJsonDocument(re).toJson();
            QVERIFY(re["is_success"].toBool() == true);
            QVERIFY(QFile::exists(re["file_path"].toString() + ".pdf"));
            QVERIFY(QFile::exists(re["file_path"].toString() + ".png"));
            auto [pages,_]     = SqliteDb::instance().getPagesDesc(0, 100);
            auto c_pageid  = re["page_id"].toInt();
            auto db_c_page = std::find_if(pages.begin(), pages.end(), [c_pageid](const auto& page) {
                return page.id == c_pageid;
            });
            QVERIFY(db_c_page != pages.end());
            QVERIFY(db_c_page->status == "LAST_PROCESS");
            QVERIFY(db_c_page->end_cmd_exec_status == 0);
            QVERIFY(db_c_page->end_cmd_exec_message.replace("\n", "") ==
                    re["file_path"].toString());
        }
        void testPrinterSpecialPaper() {

            auto config = newConfig();
            config.printer_name = PrintName;
            config.is_to_printer = 1;
            config.printer_paper_name = PaperName;
            config.printer_orientation = "Landscape";
            SqliteDb::instance().addConfig(config);
            auto page         = newPage();
            page["config_id"] = config.id;
            QEventLoop loop;

            QJsonObject re;

            std::move_only_function<void(QJsonObject)> f = [&loop, &re](QJsonObject result) {
                re = result;
                loop.quit();
                qDebug() << "Loop quit";
            };
            std::optional<int> uid = 0;
            // QSignalSpy spy(printer_work_flow->first_step.next.)
            printer_work_flow->addWorkQueue(page, std::move(f), uid);

            QTimer::singleShot(1000 * 60 * 5, &loop, &QEventLoop::quit);
            loop.exec();
            qDebug() << QJsonDocument(re).toJson();
            QVERIFY(re["is_success"].toBool() == true);
            QVERIFY(QFile::exists(re["file_path"].toString() + ".pdf"));
            QVERIFY(QFile::exists(re["file_path"].toString() + ".png"));
            auto [pages,count]     = SqliteDb::instance().getPagesDesc(0, 100);
            auto c_pageid  = re["page_id"].toInt();
            auto db_c_page = std::find_if(pages.begin(), pages.end(), [c_pageid](const auto& page) {
                return page.id == c_pageid;
            });
            QVERIFY(db_c_page != pages.end());
            QVERIFY(db_c_page->status == "LAST_PROCESS");
            QVERIFY(db_c_page->end_cmd_exec_status == 0);
            QVERIFY(db_c_page->end_cmd_exec_message.replace("\n", "") ==
                    re["file_path"].toString());
        }
#endif
};
QTEST_MAIN(TestPrint)
#include "TEST_Print.moc"
