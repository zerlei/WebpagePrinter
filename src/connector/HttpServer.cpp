#include "HttpServer.h"
#include "../exception/FatalError.h"
#include "QtNetwork/qhostaddress.h"
#include <QCoreApplication>
#include <QHttpHeaders>
#include <QHttpServerResponse>
#include <future>
#include <memory>
#include <qcoreapplication.h>
#include <qjsonobject.h>
HttpServer::HttpServer(
    std::function<void(const QString&, const QString&, const QString&, std::promise<QJsonObject>)>
                   message_handler,
    const QString& http_server_ip, const QString& http_server_port)
    : message_handler(message_handler) {

    http_server   = std::make_unique<QHttpServer>();
    tcp_server    = std::make_unique<QTcpServer>();
    auto app_path = QCoreApplication::applicationDirPath();
    http_server->route("/", [app_path]() {
        auto filepath = QString("%1/wwwroot/index.html").arg(app_path);
        return QHttpServerResponse::fromFile(filepath);
    });
    http_server->route("/assets/<arg>", [app_path](const QUrl& url) {
        auto filepath = QString("%1/wwwroot/assets/%2").arg(app_path).arg(url.path());
        return QHttpServerResponse::fromFile(filepath);
    });
    http_server->route("/command", QHttpServerRequest::Method::Post,
                       [this](const QHttpServerRequest& request) {
                           std::promise<QJsonObject> p;
                           auto                      f = p.get_future();
                           this->message_handler(request.body(), request.localAddress().toString(),
                                                 "http", std::move(p));
                           QHttpServerResponse resp(f.get());
                           QHttpHeaders        cros;
                           cros.append("Access-Control-Allow-Origin", "*");
                           cros.append("Access-Control-Allow-Methods", "GET, POST");
                           cros.append("Access-Control-Allow-Private-Network", "true");
                           cros.append("Access-Control-Allow-Credentials", "true");
                           resp.setHeaders(cros);
                           return resp;
                       });
    if (!tcp_server->listen(QHostAddress(http_server_ip), http_server_port.toUShort()) ||
        !http_server->bind(tcp_server.get())) {
        throw FatalError("Start HttpServer error");
    }
}
