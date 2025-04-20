#ifndef LOG_H
#define LOG_H

#include <QDateTime>
#include <QFile>
#include <QLoggingCategory>
#include <QMessageLogContext>
#include <QMutex>
#include <QTextStream>
// ------------------- 日志分类声明 -------------------
Q_DECLARE_LOGGING_CATEGORY(TESTLOG)

// ------------------- 初始化 & 自定义输出 -------------------
class LogManager {
public:
  static void init(const QString &filePath = "log.txt",
                   qint64 maxSizeBytes = 5 * 1024 * 1024) {
    logFilePath = filePath;
    maxSize = maxSizeBytes;

    openLogFile();

    qInstallMessageHandler(messageHandler);
  }

private:
  static inline QFile logFile;
  static inline QMutex mutex;
  static inline QString logFilePath;
  static inline qint64 maxSize = 5 * 1024 * 1024;

  static void openLogFile() {
    if (logFile.isOpen())
      logFile.close();
    logFile.setFileName(logFilePath);
    logFile.open(QIODevice::Append | QIODevice::Text);
  }

  static void rotateLogFileIfNeeded() {
    if (!logFile.isOpen())
      return;

    if (logFile.size() < maxSize)
      return;

    logFile.close();

    QString timestamp =
        QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    QString rotatedName = logFilePath;
    rotatedName.replace(".txt", "_" + timestamp + ".txt");

    QFile::rename(logFilePath, rotatedName);

    openLogFile();
  }

  static void messageHandler(QtMsgType type, const QMessageLogContext &context,
                             const QString &msg) {
    QMutexLocker locker(&mutex);
    rotateLogFileIfNeeded();

    if (!logFile.isOpen())
      return;

    QTextStream out(&logFile);
    QString timestamp =
        QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

    QString level;
    switch (type) {
    case QtDebugMsg:
      level = "DEBUG";
      break;
    case QtInfoMsg:
      level = "INFO";
      break;
    case QtWarningMsg:
      level = "WARNING";
      break;
    case QtCriticalMsg:
      level = "CRITICAL";
      break;
    case QtFatalMsg:
      level = "FATAL";
      break;
    }

    QString contextStr =
        QString("%1:%2 (%3)")
            .arg(context.file ? context.file : "unknown")
            .arg(context.line)
            .arg(context.function ? context.function : "unknown");

    out << QString("[%1] [%2] [%3] %4\n")
               .arg(timestamp)
               .arg(level)
               .arg(contextStr)
               .arg(msg);
    out.flush();
  }
};

#endif // LOG_H
