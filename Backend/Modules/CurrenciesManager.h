#pragma once
#include "utils.h"
class QSqlDatabase;

const QString currenciesTable = R"(
    CREATE TABLE currencies (
    code TEXT PRIMARY KEY,
    symbol TEXT UNIUE NOT NULL)
)";

class BACKEND_EXPORT CurrenciesManager
{
    friend class Backend;
    bool setupDefault();

    QSqlDatabase& db;
    QMap<QString, double> _currencies;
    QString _baseCurrency;
    QString apiKey;

public:
    CurrenciesManager(QSqlDatabase& db);

    QStringList codes() const { return _currencies.keys(); };
    QString base() const { return _baseCurrency; }

};

