#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include <headers/client_logger/ClientConfigReader.h>

namespace ISXConfig {

    std::optional<ClientConfig> ClientConfigReader::ReadConfig(const std::string& path)
    {
        QFile file(QString::fromStdString(path));
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Failed to open config at" << file.fileName() << ":" << file.errorString();
            return std::nullopt;
        }

        QJsonParseError parse_error;
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parse_error);
        if (parse_error.error != QJsonParseError::NoError) {
            qWarning() << "Failed to open error" << parse_error.errorString();
            return std::nullopt;
        }

        QJsonObject obj = doc.object();
        if (!obj.contains("logger")) {
            qWarning() << "No such data logger";
            return std::nullopt;
        }

        QJsonObject obj_logLevel = obj["logger"].toObject();
        if (!obj_logLevel.contains("log_level")) {
            qWarning() << "No such data log_level";
            return std::nullopt;
        }

        ClientConfig config;
        config.logLevel = static_cast<Logging::LogLevel>(obj_logLevel["log_level"].toInt());

        config.mailServerHost = "127.0.0.1";
        config.mailServerPort = "8080";
        if (obj.contains("mail_server") && obj["mail_server"].isObject()) {
            const QJsonObject mail_server = obj["mail_server"].toObject();
            config.mailServerHost = mail_server["host"].toString("127.0.0.1").toStdString();
            config.mailServerPort = QString::number(mail_server["port"].toInt(8080)).toStdString();
        }

        return config;
    }

    std::string ClientConfigReader::ConfigPath()
    {
        std::string path = std::string(CONFIG_DIR) + "/config.json";
        return path;
    }

} // namespace ISXConfig
