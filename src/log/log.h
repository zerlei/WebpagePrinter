#ifndef LOG_H
#define LOG_H

#include <QDateTime>
#include <QFile>
#include <QLoggingCategory>
#include <QMessageLogContext>
#include <QMutex>
#include <QTextStream>
// ------------------- 日志分类声明 -------------------
// NOLINTBEGIN(readability-identifier-naming)
Q_DECLARE_LOGGING_CATEGORY(APPLOG)
// NOLINTEND(readability-identifier-naming)

/**
 * @class LogManager
 * @brief
 *
 */
class LogManager {
  public:
    static void init(const QString& filePath, qint64 maxSizeBytes);

  private:
    static inline QFile   log_file;
    static inline QMutex  mutex;
    static inline QString log_file_path;
    static inline qint64  max_size = 5 * 1024 * 1024;

    static void openLogFile();
    /**
     * @brief partition logs into several log files
     */
    static void rotateLogFileIfNeeded();
    /**
     * @brief  Reprocessing message for add file name,line number etc.
     * when in relase mode , add_definitions(-DQT_MESSAGELOGCONTEXT) for record
     * extry source info
     *
     * @param type
     * @param context
     * @param msg
     */
    static void messageHandler(QtMsgType type, const QMessageLogContext& context,
                               const QString& msg);
};
#endif // LOG_H
