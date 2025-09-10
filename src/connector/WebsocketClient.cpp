#include "WebsocketClient.h"
#include <QtCore/QDebug>
#include <functional>

WebsocketClient::WebsocketClient(const QUrl& url,
                                 std::function<void(const QString&, const QString&, const QString&,
                                                    std::move_only_function<void(QJsonObject)>)>
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

    auto f = [this](QJsonObject obj) {
        auto v = QJsonDocument(obj).toJson();
        this->websocket.sendTextMessage(v);
    };
    this->message_handler(message, websocket.peerAddress().toString(), "websocket_client", f);
}