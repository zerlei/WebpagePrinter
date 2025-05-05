
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
  //! @mermaid
  //! flowchart TD
  //!     A[Christmas] -->|Get money| B(Go shopping)
  //!     B --> C{Let me think}
  //!     C -->|One| D[Laptop]
  //!     C -->|Two| E[iPhone]
  //!     C -->|Three| F[fa:fa-car Car]
  //! @endmermaid
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

  /**
   * @brief partition logs into several log files
   */
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

  /**
   * @brief  Reprocessing message for add file name,line number etc.
   * when in relase mode , add_definitions(-DQT_MESSAGELOGCONTEXT) for record
   * extry source info
   *
   * @param type
   * @param context
   * @param msg
   */
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
