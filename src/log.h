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
/**
 * @class LogManager
 * @brief
 *
 */
class LogManager {
  public:
    static void Init(const QString& filePath = "log.txt", qint64 maxSizeBytes = 5 * 1024 * 1024) {
        log_file_path = filePath;
        max_size      = maxSizeBytes;

        OpenLogFile();

        qInstallMessageHandler(MessageHandler);
    }

  private:
    /**
     * @brief LOGFILE
     * @ref openLogFile "查看这里"
    ```mermaid
    %%mermaid
    graph LR
        subgraph 对外接口
            direction LR
            Http-Server
            Websocket-Server
            Websocket-Client
        end
           对外接口-->chrome内核-->打印机驱动
    ```
     */
    static inline QFile log_file;

    /*!
    ## 13231
    ```cpp
    int main() {return 0;}
    ```
    @startuml
    Alice -> Bob
    @enduml
    */
    static inline QMutex  mutex;
    static inline QString log_file_path;
    static inline qint64  max_size = 5 * 1024 * 1024;

    static void OpenLogFile() {
        if (log_file.isOpen()) {
            log_file.close();
        }
        log_file.setFileName(log_file_path);
        log_file.open(QIODevice::Append | QIODevice::Text);
    }

    /**
     * @brief partition logs into several log files
     */
    static void RotateLogFileIfNeeded() {
        if (!log_file.isOpen()) {
            return;
        }
        if (log_file.size() < max_size) {
            return;
        }
        log_file.close();
        QString timestamp    = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
        QString rotated_name = log_file_path;
        rotated_name.replace(".txt", "_" + timestamp + ".txt");

        QFile::rename(log_file_path, rotated_name);

        OpenLogFile();
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
    static void MessageHandler(QtMsgType type, const QMessageLogContext& context,
                               const QString& msg) {
        QMutexLocker locker(&mutex);
        RotateLogFileIfNeeded();
        if (!log_file.isOpen()) {
            return;
        }
        QTextStream out(&log_file);
        QString     timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

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

        QString context_str = QString("%1:%2 (%3)")
                                  .arg(context.file ? context.file : "unknown")
                                  .arg(context.line)
                                  .arg(context.function ? context.function : "unknown");

        out << QString("[%1] [%2] [%3] %4\n").arg(timestamp).arg(level).arg(context_str).arg(msg);
        out.flush();
    }
};

#endif // LOG_H
