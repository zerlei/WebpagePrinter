#pragma once
#include "HttpServer.h"
#include "WebsocketClient.h"
#include "WebsocketServer.h"
#include <cstddef>
#include <memory>
#include <string_view>
constexpr size_t hash(std::string_view input) noexcept {
    size_t h = 0;
    for (size_t i = 0; i < input.size(); ++i) {
        h = 31 * h + static_cast<std::size_t>(input[i]);
    }
    return h;
}
class MsgStation {
  public:
    explicit MsgStation();

  private:
  void initMsgHandler();
  private:
    std::unique_ptr<HttpServer>      http_server{nullptr};
    std::unique_ptr<WebsocketServer> websocket_server{nullptr};
    std::unique_ptr<WebsocketClient> websocket_client{nullptr};
    std::function<void(const QString& msg, const QString& ip, const QString& from,
                       std::promise<QJsonObject>)> message_handler;
};