#pragma once
#include <QDir>
#include <QFile>
#include <QSettings>
#include <QString>
#include <qobject.h>
#include <tuple>

class InitConfig {

  public:
#ifdef BASE_DIR
    static const inline auto base_dir = QDir::homePath() + QString(BASE_DIR);
#else
    static const inline auto base_dir = QDir::homePath() + QString("/.WebpagePrinter/");
#endif
    static InitConfig& instance() {
        static InitConfig config{};
        return config;
    }
    InitConfig() {

        QDir dir(base_dir);
        if (dir.exists() == false) {
            dir.mkpath(base_dir);
        }
        QString config_path = QString(base_dir) + "config.ini";
        if (QFile::exists(config_path)) {
            QSettings settings(config_path, QSettings::IniFormat);
            http_server_ip   = settings.value("http_server_ip", http_server_ip).toString();
            http_server_port = settings.value("http_server_port", http_server_port).toString();
            local_websocket_server_ip =
                settings.value("local_websocket_server_ip", local_websocket_server_ip).toString();
            local_websocket_server_port =
                settings.value("local_websocket_server_port", local_websocket_server_port)
                    .toString();
            remote_websocket_server_url_only_ws =
                settings.value("remote_websocket_server_ip", remote_websocket_server_url_only_ws)
                    .toString();
        } else {
            QSettings settings(config_path, QSettings::IniFormat);
            settings.setValue("http_server_ip", http_server_ip);
            settings.setValue("http_server_port", http_server_port);
            settings.setValue("local_websocket_server_ip", local_websocket_server_ip);
            settings.setValue("local_websocket_server_port", local_websocket_server_port);
            settings.setValue("remote_websocket_server_url_only_ws",
                              remote_websocket_server_url_only_ws);
            settings.sync();
        }
    }

    std::tuple<const QString&, const QString&> getHttpServerIpPort() const {
        return std::make_tuple(http_server_ip, http_server_port);
    }
    std::tuple<const QString&, const QString&> getLocalWebsocketServerIpPort() const {
        return std::make_tuple(local_websocket_server_ip, local_websocket_server_port);
    }
    const QString& getRemoteWebsocketServerUrl() const {
        return remote_websocket_server_url_only_ws;
    }

  private:
    QString http_server_ip{"0.0.0.0"};
    QString http_server_port{"8845"};
    QString local_websocket_server_ip{"0.0.0.0"};
    QString local_websocket_server_port{"8846"};
    QString remote_websocket_server_url_only_ws{""};
};
