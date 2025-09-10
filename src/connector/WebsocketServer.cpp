#include "WebsocketServer.h"
#include <QJsonObject>
#include <QtCore/QDebug>
#include <functional>
#include <qjsondocument.h>

WebsocketServer::WebsocketServer(const QString& host_address, const QString& port,
                                 std::function<void(const QString&, const QString&, const QString&,
                                                    std::move_only_function<void(QJsonObject)>)>
                                          message_handler,
                                 QObject* parent)
    : QObject(parent)
    , websocket_server(
          new QWebSocketServer(QStringLiteral(""), QWebSocketServer::NonSecureMode, this))
    , message_handler(message_handler) {
    if (websocket_server->listen(QHostAddress(host_address), port.toUShort())) {
        connect(websocket_server, &QWebSocketServer::newConnection, this,
                &WebsocketServer::onNewConnection);
        connect(websocket_server, &QWebSocketServer::closed, this, &WebsocketServer::closed);
    }
}

WebsocketServer::~WebsocketServer() {
    websocket_server->close();
    qDeleteAll(clients.begin(), clients.end());
}

void WebsocketServer::onNewConnection() {
    QWebSocket* socket = websocket_server->nextPendingConnection();

    connect(socket, &QWebSocket::textMessageReceived, this, &WebsocketServer::processTextMessage);
    connect(socket, &QWebSocket::disconnected, this, &WebsocketServer::socketDisconnected);
    clients.push_back(socket);
}
void WebsocketServer::processTextMessage(QString message) {
    QWebSocket* client = qobject_cast<QWebSocket*>(sender());
    if (client) {
        auto f = [client](QJsonObject obj) {
            auto v = QJsonDocument(obj).toJson();
            client->sendTextMessage(v);
        };
        this->message_handler(message, client->peerAddress().toString(), "websocket_server", f);
    }
}

void WebsocketServer::socketDisconnected() {
    QWebSocket* client = qobject_cast<QWebSocket*>(sender());
    if (client) {
        clients.removeAll(client);
        client->deleteLater();
    }
}