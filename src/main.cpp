// #include "log/log.h"
#include "QtNetwork/qhostaddress.h"
#include <QApplication>
#include <QDebug>
#include <QtHttpServer/QtHttpServer>
#include <QTcpServer>
#include <QWidget>
#include <qlogging.h>
using namespace Qt::StringLiterals;
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
    QHttpServer  http_server;
    http_server.route("/t", []() {
        return "Hello world";
    });
    http_server.route("/", []() {
        return QHttpServerResponse::fromFile(
            QString("/home/zerlei/git/WebpagePrinter/tests/TEST_httpserver/index.html"));
    });
    http_server.route("/assets/<arg>", [](const QUrl& url) {
        return QHttpServerResponse::fromFile(
            QString("/home/zerlei/git/WebpagePrinter/tests/TEST_httpserver/assets/") + url.path());
    });

    auto tcpserver = new QTcpServer();
    if (!tcpserver->listen(QHostAddress::Any, 8845) || !http_server.bind(tcpserver)) {
        delete tcpserver;
        return -1;
    }
    // QWidget      w;
    // w.show();
    // LogManager::init("log.txt", 5 * 1024 * 1024); // 每 5 MB 切割一次
    // qCDebug(AppLog) << "Application started";
    // qCWarning(NetLog) << "Network delay detected";
    // qCCritical(UiLog) << "UI failed to load";

    return QApplication::exec();
}
