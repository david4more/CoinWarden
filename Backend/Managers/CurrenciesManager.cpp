#include "CurrenciesManager.h"
#include "NetworkManager.h"
#include "../Modules/pch.h"
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonObject>
#include <QtWidgets/qmessagebox.h>
#include <QSqlError>

CurrenciesManager::CurrenciesManager(QSqlDatabase& db) : db(db)
{
    _base = "EUR";
    QSqlQuery query(db);
    apiKey = qgetenv("CURRENCY_API_KEY");
    Q_ASSERT(!apiKey.isEmpty());
    if (!query.exec("SELECT 1 FROM currencies LIMIT 1")) { qDebug() << query.lastError().text(); return; }

    query.prepare("SELECT * FROM currencies");
    if (!query.exec()) qDebug() << "Failed to execute CurrenciesManager::CurrenciesManager query";
    else
        while (query.next()) {
            _currencies[query.value(0).toString()] = query.value(1).toDouble();
            _symbols[query.value(0).toString()] = query.value(2).toString()[0];
        }
}

bool CurrenciesManager::setupDefault()
{
    QString currencies = "EUR,GBP,CHF,PLN,UAH,USD,CAD,JPY,CNY";
    _symbols["EUR"] = QChar(0x20AC); // €
    _symbols["GBP"] = QChar(0x00A3); // £
    _symbols["CHF"] = QChar(0x0043); // C (no single-char symbol, maybe "CHF" string is better)
    _symbols["PLN"] = QChar(0x007A); // z (or just use "PLN" string)
    _symbols["UAH"] = QChar(0x20B4); // ₴
    _symbols["USD"] = QChar(0x0024); // $
    _symbols["CAD"] = QChar(0x0024); // $ (same as USD, distinguish with code)
    _symbols["JPY"] = QChar(0x00A5); // ¥
    _symbols["CNY"] = QChar(0x00A5); // ¥ (same as JPY, distinguish with code)

    QUrl url("https://api.currencyapi.com/v3/latest");
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("currencies", currencies);
    urlQuery.addQueryItem("base_currency", _base);
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    request.setRawHeader("apikey", apiKey.toUtf8());

    QJsonObject rootObj = NetworkManager::blockingNetworkRequest(request); qDebug() << "CurrencyAPI was called";
    QJsonObject obj = rootObj["data"].toObject();

    QString lastUpdated = rootObj["meta"].toObject()["last_updated_at"].toString();
    lastUpdate = QDateTime::fromString(lastUpdated, Qt::ISODate);

    QSqlQuery query(db);

    auto names = obj.keys();

    if (!db.transaction()) { qDebug() << "Failed to initialize a transaction"; return false; }

    query.prepare("INSERT INTO currencies (code, rate, symbol) VALUES (:code, :rate, :symbol)");
    for (int i = 0; i < obj.size(); ++i) {
        QString code = names[i];
        double rate = obj[code].toObject()["value"].toDouble();
        QChar symbol = _symbols[code];
        _currencies[code] = rate;
        query.bindValue(":code", code);
        query.bindValue(":rate", rate);
        query.bindValue(":symbol", QString(symbol));
        query.exec();
    }

    if (!db.commit()) { qDebug() << "Failed to commit transaction"; return false; }

    return true;
}
