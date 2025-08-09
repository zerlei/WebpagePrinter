#pragma once
#include "QtNetwork/qtcpserver.h"
#include <QtHttpServer/QtHttpServer>
#include <functional>
#include <future>
#include <memory>
#include <qjsonobject.h>

class HttpServer {
  public:
  /**
   * @brief Construct a new Http Server object
   * 
   * @param message_handler 
   * @param http_server_ip 
   * @param http_server_port 
   */
    explicit HttpServer(
        std::function<void(const QString&, const QString&, const QString&,std::promise<QJsonObject>)> message_handler,
        const QString& http_server_ip, const QString& http_server_port);

  private:
    std::unique_ptr<QHttpServer> http_server{nullptr};
    std::unique_ptr<QTcpServer>  tcp_server{nullptr};
    std::function<void(const QString&, const QString&, const QString&, std::promise<QJsonObject>)>
        message_handler;
};
