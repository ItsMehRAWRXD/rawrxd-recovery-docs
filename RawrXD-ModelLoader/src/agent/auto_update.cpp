#include "auto_update.hpp"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCryptographicHash>
#include <QProcess>
#include <QEventLoop>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QUrl>

static const char* UPDATE_URL = "https://rawrxd.blob.core.windows.net/updates/update_manifest.json";

bool AutoUpdate::checkAndInstall() {
    QNetworkAccessManager nam;
    QNetworkRequest req;
    req.setUrl(QUrl(QString::fromUtf8(UPDATE_URL)));
    QNetworkReply* reply = nam.get(req);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Update: cannot fetch manifest" << reply->errorString();
        reply->deleteLater();
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    reply->deleteLater();
    QJsonObject root = doc.object();
    QString remoteVer = root["version"].toString();
    QString remoteURL = root["url"].toString();
    QString remoteSHA = root["sha256"].toString();

    QString localVer = QApplication::applicationVersion();
    if (remoteVer == localVer) {
        qInfo() << "Update: already on" << remoteVer;
        return true;
    }

    QString localPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                        + "/updates/RawrXD-QtShell-" + remoteVer + ".exe";
    QDir().mkpath(QFileInfo(localPath).absolutePath());

    QUrl dlUrl(remoteURL);
    QNetworkRequest dlReq;
    dlReq.setUrl(dlUrl);
    QNetworkReply* dlReply = nam.get(dlReq);
    connect(dlReply, &QNetworkReply::downloadProgress,
            [](qint64 received, qint64 total) {
                qInfo() << "Update: download" << received << "/" << total;
            });
    connect(dlReply, &QNetworkReply::finished, [dlReply, localPath, remoteSHA]() {
        QByteArray data = dlReply->readAll();
        dlReply->deleteLater();
        QString sha256 = QCryptographicHash::hash(data, QCryptographicHash::Sha256).toHex();
        if (sha256 != remoteSHA) {
            qWarning() << "Update: SHA256 mismatch";
            return;
        }
        QFile f(localPath);
        if (f.open(QIODevice::WriteOnly)) {
            f.write(data);
            f.close();
            qInfo() << "Update: downloaded" << localPath;
            QStringList args = {"/C", "timeout", "/t", "3", "&&", localPath};
            QProcess::startDetached("cmd.exe", args);
            QCoreApplication::quit();
        }
    });

    return true;
}
