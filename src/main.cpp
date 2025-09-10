#include "InitConfig.h"
#include "connector/MsgStation.h"
#include "db/SqliteDb.h"
#include "excep/FatalError.h"
#include "log/log.h"
#include <QApplication>
#include <QDebug>
#include <QHttpServerResponder>
#include <QUrl>
#include <QWebEngineView>
#include <QWidget>
int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    LogManager::init(InitConfig::base_dir + "log.txt", 5 * 1024 * 1024);
    QObject::connect(&a, &QApplication::aboutToQuit, []() { qCInfo(APPLOG) << "Normal Exit..."; });
    [[maybe_unused]] MsgStation* msg_station;
    try {
        InitConfig::instance();
        SqliteDb::instance();
        msg_station = new MsgStation();
    } catch (const FatalError& e) {
        qCFatal(APPLOG) << e.what();
    } catch (...) {
        qCFatal(APPLOG) << "Unknown error";
    }
    qCInfo(APPLOG) << "Start...";
    return QApplication::exec();
}

// int main(int argc, char* argv[]) {
//     QApplication a(argc, argv);

//     QHttpServer http_server;
//     QTcpServer tcp_server;

//     http_server.route("/t", [](const QHttpServerRequest&, QHttpServerResponder& res) {
//         auto f = [resp = std::move(res)]() mutable {
//             QJsonObject obj;
//             obj["connect_test"] = "hello";
//             QJsonDocument doc(obj);
//             resp.write(doc);
//         };
//         f();
//     });
//     if (!tcp_server.listen(QHostAddress("0.0.0.0"),QString("8845").toUShort() ) ||
//         !http_server.bind(&tcp_server)) {
//         throw FatalError("Start HttpServer error");
//     }
//     return QApplication::exec();
// }
