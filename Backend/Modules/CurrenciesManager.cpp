#include "CurrenciesManager.h"
#include "NetworkManager.h"
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonObject>

CurrenciesManager::CurrenciesManager(QSqlDatabase& db) : db(db)
{
    _baseCurrency = "EUR";
    QString currencies = "EUR,GBP,CHF,PLN,UAH,USD,CAD,JPY,CNY";
    for (auto x : currencies.split(',')) _currencies.insert(x, 0.0);
    apiKey = "cur_live_8hmo833CJvaq1hSZ5DW2vy5md8ZFRWKpOp5CiEGY";
}

bool CurrenciesManager::setupDefault()
{
    QString currencies = _currencies.keys().join(",");
    qDebug() << currencies;

    QUrl url("https://api.currencyapi.com/v3/latest");
    QUrlQuery query;
    query.addQueryItem("currencies", currencies);
    query.addQueryItem("base_currency", _baseCurrency);
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("apikey", apiKey.toUtf8());

    // QJsonObject obj = NetworkManager::executeNetworkRequest(request);

    // move ts to onFirstLaunch, handle work with currencies table


    qDebug() << "oops\noopsy\noopsydoopsy";

    return true;
}
