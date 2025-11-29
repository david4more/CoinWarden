#include "NetworkManager.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QJsonObject>
#include <QNetworkRequest>


QJsonObject NetworkManager::executeNetworkRequest(QNetworkRequest request)
{
    QNetworkAccessManager* mgr = new QNetworkAccessManager(this);

    connect(mgr, &QNetworkAccessManager::finished, this, [this](QNetworkReply* reply) {
        if (reply->error() != QNetworkReply::NoError) { qDebug() << reply->errorString(); return; }

        QByteArray data;
        data = reply->readAll();
        reply->deleteLater();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (!doc.isObject()) { qDebug() << "Invalid JSON!"; return; }

        QJsonObject obj = doc.object();
        //for (auto x : obj.keys()) _currencies[x] = obj[x].toDouble();
    });

    mgr->get(request);

    return {};
}
