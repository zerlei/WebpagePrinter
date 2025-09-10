#pragma once
#include <QList>
#include <QtCore/QByteArray>
#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>
#include <functional>

class WebsocketServer : public QObject {
    Q_OBJECT
      public:
        explicit WebsocketServer(
            const QString& host_address, const QString& port,
            std::function<void(const QString& port, const QString&, const QString&,
                               std::move_only_function<void(QJsonObject)>)>
                     message_handler,
            QObject* parent = nullptr);
        ~WebsocketServer();

      Q_SIGNALS:
        void closed();

      private Q_SLOTS:
        void onNewConnection();
        void processTextMessage(QString message);
        void socketDisconnected();

      private:
        QWebSocketServer* websocket_server;
        /**
         * @brief thread safe
         *
         */
        QList<QWebSocket*> clients;
        std::function<void(const QString&, const QString&, const QString&,
                           std::move_only_function<void(QJsonObject)>)>
            message_handler;
};