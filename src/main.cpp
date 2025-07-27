#include "log/log.h"
#include <QApplication>
#include <QDebug>
#include <QWidget>

// Q_LOGGING_CATEGORY(AppLog, "app")
// Q_LOGGING_CATEGORY(NetLog, "app.net")
// Q_LOGGING_CATEGORY(UiLog, "app.ui")
//
/**
 * @brief 你好 这是main 函数
 *
 * @param argc  传递多少个参数
 * @param argv  传递的参数
 * @return int 返回值
 */
int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    QWidget      w;
    w.show();
    // LogManager::init("log.txt", 5 * 1024 * 1024); // 每 5 MB 切割一次
    // qCDebug(AppLog) << "Application started";
    // qCWarning(NetLog) << "Network delay detected";
    // qCCritical(UiLog) << "UI failed to load";

    return QApplication::exec();
}
