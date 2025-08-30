#include "InitConfig.h"
#include "connector/MsgStation.h"
#include "db/SqliteDb.h"
#include "exception/FatalError.h"
#include "log/log.h"
#include <QApplication>
#include <QDebug>
#include <QUrl>
#include <QWebEngineView>
#include <QWidget>
int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    LogManager::init(InitConfig::base_dir + "log.txt", 5 * 1024 * 1024);
    QObject::connect(&a, &QApplication::aboutToQuit, []() { qCInfo(APPLOG) << "Normal Exit..."; });
    try {
        InitConfig::instance();
        SqliteDb::instance();
        MsgStation msg_station;
    } catch (const FatalError& e) {
        qCFatal(APPLOG) << e.what();
    } catch (...) {
        qCFatal(APPLOG) << "Unknown error";
    }
    qCInfo(APPLOG) << "Start...";

    return QApplication::exec();
}
