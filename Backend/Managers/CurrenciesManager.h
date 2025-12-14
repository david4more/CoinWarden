#pragma once
#include "../Modules/Utils.h"
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
    QMap<QString, QChar> _symbols;
    QString _base;
    QString apiKey;
    QDateTime lastUpdate;

public:
    CurrenciesManager(QSqlDatabase& db);

    QStringList codes() const { auto codes = _currencies.keys(); codes.removeAll(_base); codes.prepend(_base); return codes; }
    QMap<QString, double> currencies() const { return _currencies; }
    QMap<QString, QChar> symbols() const { return _symbols; }
    QString base() const { return _base; }
    double value(QString key) const { return _currencies.value(key, 0.0); }
};

