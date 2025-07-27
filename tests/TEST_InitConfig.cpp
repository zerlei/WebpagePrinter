#include "../src/InitConfig.h"
#include <QDir>
#include <QObject>
#include <qtest.h>
#include <qtestcase.h>

class TestInitConfig : public QObject {

    Q_OBJECT
      private:
        InitConfig* init_config{nullptr};

      private slots:
        void initTestCase() {
            QDir dir(InitConfig::base_dir);
            if (dir.exists()) {
                dir.removeRecursively();
            }
        }
        /**
         * @brief Recursively removes the base directory used by InitConfig.
         * 
         * This is a cleanup helper function that deletes the InitConfig's base directory
         * and all its contents if it exists. Typically used in test teardown.
         */
        void cleanup() {
            QDir dir(InitConfig::base_dir);
            if (dir.exists()) {
                dir.removeRecursively();
            }
        }

        void checkInitConfig() {
            QVERIFY(!QFile::exists(QString(InitConfig::base_dir) + "config.ini"));
            init_config = new InitConfig();
            QVERIFY(QFile::exists(QString(InitConfig::base_dir) + "config.ini"));
            auto [http_server_ip, http_server_port] = init_config->getHttpServerIpPort();
            auto [local_websocket_server_ip, local_websocket_server_port] =
                init_config->getLocalWebsocketServerIpPort();
            auto [remote_websocket_server_ip, remote_websocket_server_port] =
                init_config->getRemoteWebsocketServerIpPort();
            QVERIFY(http_server_ip == "0.0.0.0");
            QVERIFY(http_server_port == "8845");
            QVERIFY(local_websocket_server_ip == "0.0.0.0");
            QVERIFY(local_websocket_server_port == "8846");
            QVERIFY(remote_websocket_server_ip == "");
            QVERIFY(remote_websocket_server_port == "");
            delete init_config;
            QSettings settings(QString(InitConfig::base_dir) + "config.ini",
                               QSettings::IniFormat);
            settings.setValue("http_server_ip", "127.0.0.1");
            settings.setValue("http_server_port", "7777");
            settings.setValue("local_websocket_server_ip", "192.168.0.5");
            settings.setValue("local_websocket_server_port", "7778");
            settings.setValue("remote_websocket_server_ip", "212.129.23.12");
            settings.setValue("remote_websocket_server_port", "89");
            settings.sync();
            {

                QVERIFY(QFile::exists(QString(InitConfig::base_dir) + "config.ini"));
                init_config = new InitConfig();
                QVERIFY(QFile::exists(QString(InitConfig::base_dir) + "config.ini"));

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
                auto [http_server_ip, http_server_port] = init_config->getHttpServerIpPort();
                auto [local_websocket_server_ip, local_websocket_server_port] =
                    init_config->getLocalWebsocketServerIpPort();
                auto [remote_websocket_server_ip, remote_websocket_server_port] =
                    init_config->getRemoteWebsocketServerIpPort();
#pragma clang diagnostic pop
                QVERIFY(http_server_ip == "127.0.0.1");
                QVERIFY(http_server_port == "7777");
                QVERIFY(local_websocket_server_ip == "192.168.0.5");
                QVERIFY(local_websocket_server_port == "7778");
                QVERIFY(remote_websocket_server_ip == "212.129.23.12");
                QVERIFY(remote_websocket_server_port == "89");
            }
        }
};
QTEST_MAIN(TestInitConfig)
#include "TEST_InitConfig.moc"