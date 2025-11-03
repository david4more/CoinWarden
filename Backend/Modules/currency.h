#pragma once

class QSqlDatabase;

const QString currenciesTable =
    "CREATE TABLE currencies ("
    "code TEXT PRIMARY KEY, "
    "symbol TEXT UNIUE NOT NULL)";


class Currency
{
    QString name, code, symbol;
    double rate;

public:
    Currency(QString n, QString c, QString s, double r) : name(n), code(c), symbol(s), rate(r) {}

    double convertToDefault(double amount) { return amount * rate; };
};

class CurrenciesManager
{
    QSqlDatabase& db;
    QStringList _currencies;
    QString _defaultCurrency;

public:
    CurrenciesManager(QSqlDatabase& db);

    QStringList codes() const { return _currencies; };
    QString def() const { return _defaultCurrency; }
};

