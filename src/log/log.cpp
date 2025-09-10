#include "log.h"
Q_LOGGING_CATEGORY(APPLOG, "APP")
void LogManager::init(const QString& filePath = "log.txt", qint64 maxSizeBytes = 5 * 1024 * 1024) {
    log_file_path = filePath;
    max_size      = maxSizeBytes;
    openLogFile();
    qInstallMessageHandler(messageHandler);
}
void LogManager::openLogFile() {
    if (log_file.isOpen()) {
        log_file.close();
    }
    log_file.setFileName(log_file_path);
    log_file.open(QIODevice::Append | QIODevice::Text);
}
void LogManager::rotateLogFileIfNeeded() {
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

    openLogFile();
}
void LogManager::messageHandler(QtMsgType type, const QMessageLogContext& context,
                                const QString& msg) {
    QMutexLocker locker(&mutex);
    rotateLogFileIfNeeded();
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