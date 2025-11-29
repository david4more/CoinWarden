#pragma once
#include "utils.h"
class QSqlDatabase;

const QString currenciesTable = R"(
    CREATE TABLE currencies (
    code TEXT PRIMARY KEY,
    rate REAL NOT NULL,
    symbol TEXT)
)";

class BACKEND_EXPORT CurrenciesManager
{
    friend class Backend;
    bool setupDefault();

    QSqlDatabase& db;
    QMap<QString, double> _currencies;
    QString _baseCurrency;
    QString apiKey;
    QDateTime lastUpdate;

public:
    CurrenciesManager(QSqlDatabase& db);

    QStringList codes() const { return _currencies.keys(); };
    QMap<QString, double> currencies() const { return _currencies; }
    QString base() const { return _baseCurrency; }
};

