#include "../src/log.h"
#include <QFile>
#include <QString>
#include <QTest>
#include <QTextStream>
#include <array>
#include <qloggingcategory.h>
#include <qtestcase.h>
Q_LOGGING_CATEGORY(TESTLOG, "test.log")
class TestLog : public QObject {
  Q_OBJECT
private:
  /**
   * @brief rmove all log files for a clean test env
   */
  void removeLogFile() {

    QDir dir("./");

    // 获取目录中的所有文件（不包括子文件夹）
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::NoSymLinks);

    for (const QFileInfo &fileInfo : fileList) {
      QString fileName = fileInfo.fileName();

      if (fileName.endsWith(".txt", Qt::CaseInsensitive) &&
          fileName.contains("log", Qt::CaseInsensitive)) {

        QString fullPath = fileInfo.absoluteFilePath();
        if (!QFile::remove(fullPath)) {
          QFAIL(QString("remove file : %1 fail !")
                    .arg(fullPath)
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
  void cleanup() { removeLogFile(); }
  /**
   * @brief test log writer correct
   */
  void testLog() {
    qCDebug(TESTLOG) << "DEBUG";
    qCInfo(TESTLOG) << "INFO";
    qCWarning(TESTLOG) << "WARNING";
    qCCritical(TESTLOG) << "CRITICAL";
    // qCFatal(TESTLOG) << "FATAL";
    QFile file("log.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      QFAIL("Failed to open log file");
    }
    QTextStream in(&file);
    int c = 0;
    std::array<QString, 4> typeInfo{"DEBUG", "INFO", "WARNING", "CRITICAL"};
    while (!in.atEnd()) {
      QString line = in.readLine();
      if (!(line.contains("TEST_log") && line.contains(typeInfo.at(c)))) {
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
      qCDebug(TESTLOG) << "DEBUG";
      qCInfo(TESTLOG) << "INFO";
      qCWarning(TESTLOG) << "WARNING";
      qCCritical(TESTLOG) << "CRITICAL";
    }
    QDir dir("./");

    // 获取目录中的所有文件（不包括子文件夹）
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::NoSymLinks);

    short numbers = 0;
    for (const QFileInfo &fileInfo : fileList) {
      QString fileName = fileInfo.fileName();

      if (fileName.endsWith(".txt", Qt::CaseInsensitive) &&
          fileName.contains("log", Qt::CaseInsensitive)) {
        numbers++;
      }
    }
    QVERIFY(numbers > 1);
  }
};
QTEST_MAIN(TestLog)
#include "TEST_log.moc"
