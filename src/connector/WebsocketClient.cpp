#include "WebsocketClient.h"
#include <QtCore/QDebug>

WebsocketClient::WebsocketClient(
    const QUrl& url,
    std::function<void(const QString&, const QString&, const QString&, std::promise<QJsonObject>)>
        message_handler)
    : message_handler(message_handler) {
    connect(&websocket, &QWebSocket::connected, this, &WebsocketClient::onConnected);
    connect(&websocket, &QWebSocket::disconnected, this, &WebsocketClient::closed);
    websocket.open(url);
}
void WebsocketClient::onConnected() {
    connect(&websocket, &QWebSocket::textMessageReceived, this,
            &WebsocketClient::onTextMessageReceived);
}
void WebsocketClient::onTextMessageReceived(QString message) {

    std::promise<QJsonObject> p;
    auto                      f = p.get_future();
    this->message_handler(message, websocket.peerAddress().toString(), "websocket_client",
                          std::move(p));
    auto v = QJsonDocument(f.get()).toJson();
    websocket.sendTextMessage(v);
}