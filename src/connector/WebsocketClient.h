#pragma once
#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <functional>
#include <qjsonobject.h>

class WebsocketClient : public QObject {
    Q_OBJECT
      public:
        explicit WebsocketClient(const QUrl& url,
                                 std::function<void(const QString&, const QString&, const QString&,
                                                    std::move_only_function<void(QJsonObject)>)>
                                     message_handler);

      Q_SIGNALS:
        void closed();

      private Q_SLOTS:
        void onConnected();
        void onTextMessageReceived(QString message);

      private:
        std::function<void(const QString&, const QString&, const QString&,
                           std::move_only_function<void(QJsonObject)>)>
                   message_handler;
        QWebSocket websocket;
};