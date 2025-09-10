#include "../src/log/log.h"
#include <QFile>
#include <QString>
#include <QTest>
#include <QTextStream>
#include <array>
#include <cstddef>
#include <qloggingcategory.h>
#include <qnumeric.h>
#include <qtestcase.h>
class TestPrint : public QObject {
    Q_OBJECT
      private:
        /**
         * @brief rmove all log files for a clean test env
         */
        void removeLogFile() {

            QDir dir("./");

            // 获取目录中的所有文件（不包括子文件夹）
            QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::NoSymLinks);

            for (const QFileInfo& file_info : file_list) {
                QString file_name = file_info.fileName();

                if (file_name.endsWith(".txt", Qt::CaseInsensitive) &&
                    file_name.contains("log", Qt::CaseInsensitive)) {
                    QString full_path = file_info.absoluteFilePath();
                    if (!QFile::remove(full_path)) {
                        QFAIL(QString("remove file : %1 fail !")
                                  .arg(full_path)
                                  .toStdString()
                                  .c_str());
                    }
                }
            }
        }

      private slots:
        void initTestCase() {
            removeLogFile();
            LogManager::init("log.txt", 1024);
        }
        void cleanup() {
            removeLogFile();
        }
        /**
         * @brief test log writer correct
         */
        void testLoger() {
            qCDebug(APPLOG) << "DEBUG";
            qCInfo(APPLOG) << "INFO";
            qCWarning(APPLOG) << "WARNING";
            qCCritical(APPLOG) << "CRITICAL";
            // qCFatal(APPLOG) << "FATAL";
            QFile file("log.txt");
            if (!file.exists()) {
                QFAIL("File not exist");
            }
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QFAIL("Open file fail");
            }
            QTextStream            in(&file);
            int                    c = 0;
            std::array<QString, 4> type_info{"DEBUG", "INFO", "WARNING", "CRITICAL"};
            while (!in.atEnd()) {
                QString line = in.readLine();
                if (!(line.contains("TEST_log") &&
                      line.contains(type_info[static_cast<size_t>(c)]))) {
                    QFAIL("forget add_definitions(-DQT_MESSAGELOGCONTEXT) in cmake ?");
                }
                c++;
            }
            if (c != 4) {
                QFAIL(QString("Line Numbers %1 error").arg(c).toStdString().c_str());
            }
        }
        /*
         * @brief partition logs into several log files
         */
        void testLogMutiLogs() {
            for (int i = 0; i < 10; i++) {
                qCDebug(APPLOG) << "DEBUG";
                qCInfo(APPLOG) << "INFO";
                qCWarning(APPLOG) << "WARNING";
                qCCritical(APPLOG) << "CRITICAL";
            }
            QDir dir("./");

            // 获取目录中的所有文件（不包括子文件夹）
            QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::NoSymLinks);

            short numbers = 0;
            for (const QFileInfo& file_info : file_list) {
                QString file_name = file_info.fileName();
                if (file_name.endsWith(".txt", Qt::CaseInsensitive) &&
                    file_name.contains("log", Qt::CaseInsensitive)) {
                    numbers++;
                }
            }
            QVERIFY(numbers > 1);
        }
};
QTEST_MAIN(TestPrint)
#include "TEST_log.moc"
