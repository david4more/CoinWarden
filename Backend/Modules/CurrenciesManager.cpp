#include "CurrenciesManager.h"
#include "NetworkManager.h"
#include "pch.h"
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonObject>
#include <QtWidgets/qmessagebox.h>

CurrenciesManager::CurrenciesManager(QSqlDatabase& db) : db(db)
{
    _baseCurrency = "EUR";
    QString currencies = "EUR,GBP,CHF,PLN,UAH,USD,CAD,JPY,CNY";
    for (auto x : currencies.split(',')) _currencies.insert(x, 0.0);
    apiKey = qgetenv("CURRENCY_API_KEY");
    Q_ASSERT(!apiKey.isEmpty());
}

bool CurrenciesManager::setupDefault()
{
    QUrl url("https://api.currencyapi.com/v3/latest");
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("currencies", _currencies.keys().join(","));
    urlQuery.addQueryItem("base_currency", _baseCurrency);
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    request.setRawHeader("apikey", apiKey.toUtf8());

    QJsonObject rootObj = NetworkManager::blockingNetworkRequest(request); qDebug() << "CurrencyAPI was called";
    QJsonObject obj = rootObj["data"].toObject();

    QString lastUpdated = rootObj["meta"].toObject()["last_updated_at"].toString();
    lastUpdate = QDateTime::fromString(lastUpdated, Qt::ISODate);
    qDebug() << "Last update: " << lastUpdate.toString();

    QSqlQuery query(db);

    auto names = obj.keys();

    if (!db.transaction()) { qDebug() << "Failed to initialize a transaction"; return false; }

    query.prepare("INSERT INTO currencies (code, rate) VALUES (:code, :rate)");
    for (int i = 0; i < obj.size(); ++i) {
        QString code = names[i];
        double rate = obj[code].toObject()["value"].toDouble();
        query.bindValue(":code", code);
        query.bindValue(":rate", rate);
        query.exec();
    }

    if (!db.commit()) { qDebug() << "Failed to commit transaction"; return false; }

    return true;
}
