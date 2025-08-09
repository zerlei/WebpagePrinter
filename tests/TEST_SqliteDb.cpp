#include "../src/InitConfig.h"
#include "../src/db/SqliteDb.h"
#include <QObject>
#include <qdir.h>
#include <qtest.h>
#include <qtestcase.h>
class TestSqliteDb : public QObject {
    Q_OBJECT
      private:
        /**
         * @brief  InitConfig init_config{} error!!! construct won't be triggered;
         *
         */
        InitConfig* init_config{nullptr};
      private slots:

        void initTestCase() {
            QDir dir(InitConfig::base_dir);
            if (dir.exists()) {
                dir.removeRecursively();
            }
            init_config = new InitConfig();
        }

        void checkCreateDb() {
            QDir dir(InitConfig::base_dir);
            QVERIFY(dir.exists());
            QVERIFY(!QFile::exists(QString(InitConfig::base_dir) + "data.db"));
            [[maybe_unused]] auto& db = SqliteDb::instance();
            QVERIFY(QFile::exists(QString(InitConfig::base_dir) + "data.db"));
        }
        void checkConfigOp() {
            PrinterConfig config{};
            config.id                            = 0;
            config.name                          = "Test_PrinterName";
            config.is_save_png                   = 0;
            config.is_to_printer                 = 1;
            config.width_mm                      = 10;
            config.height_mm                     = 20;
            config.left_margin                   = 0.0;
            config.right_margin                  = 1;
            config.top_margin                    = 4;
            config.bottom_margin                 = 2;
            config.printer_name                  = "TestdsfName";
            config.is_use_printer_default_config = 0;
            config.printer_paper_name            = "A4";
            config.printer_orientation           = "Auto";
            config.cmd_at_successs_end           = "dfadsf";
            SqliteDb::instance().addConfig(config);

            PrinterConfig config2 = SqliteDb::instance().getConfigById(config.id);
            QVERIFY(config2 == config);
            config.name                          = "Test_PrinterName1";
            config.is_save_png                   = 1;
            config.is_to_printer                 = 0;
            config.width_mm                      = 11;
            config.height_mm                     = 21;
            config.left_margin                   = 1;
            config.right_margin                  = 2;
            config.top_margin                    = 3;
            config.bottom_margin                 = 4;
            config.printer_name                  = "TestdsfName1";
            config.is_use_printer_default_config = 1;
            config.printer_paper_name            = "A45";
            config.printer_orientation           = "Autosdf";
            config.cmd_at_successs_end           = "vsf";

            SqliteDb::instance().updateConfig(config);
            auto config3 = SqliteDb::instance().getConfigById(config.id);
            QVERIFY(config3 == config);
            auto cs = SqliteDb::instance().getAllConfigs();
            QVERIFY(config == cs.back());
            SqliteDb::instance().delConfig(config.id);
            auto cs2 = SqliteDb::instance().getAllConfigs();
            QVERIFY(cs2.empty());

            SqliteDb::instance().addConfig(config);
            SqliteDb::instance().addConfig(config);
            SqliteDb::instance().addConfig(config);
            SqliteDb::instance().addConfig(config);
            // std::vector 会在扩容时调用内部元素的移动构造函数
            auto cs3 = SqliteDb::instance().getAllConfigs();

            QVERIFY(cs3.size() == 4);
        }
        void checkPageOp() {
            PrintedPage page{};
            page.id                   = 0;
            page.config_id            = 1;
            page.time                 = "2013";
            page.status               = "UUU";
            page.from_ip              = "1.1.1.1";
            page.page_url             = "bing.com";
            page.end_cmd_exec_message = "susuuuuususufsdkfsfdjksadjfjsdfjklfklasjdlfkjsdkljfl";

            SqliteDb::instance().addPage(page);
            auto ps = SqliteDb::instance().getPagesDesc(1, 10);
            QVERIFY(page == ps.back());
            SqliteDb::instance().addPage(page);
            SqliteDb::instance().addPage(page);
            SqliteDb::instance().addPage(page);
            auto ps2 = SqliteDb::instance().getPagesDesc(1, 10);
            QVERIFY(ps2.size() == 4);
        }
};
QTEST_MAIN(TestSqliteDb)
#include "TEST_SqliteDb.moc"