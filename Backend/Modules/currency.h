#pragma once

#include <QStringList>
#include <QMap>


const QString currenciesTable =
    "CREATE TABLE IF NOT EXISTS currencies ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "name TEXT UNIQUE NOT NULL,"
    "symbol TEXT UNIUE NOT NULL)";


class Currency;
class CurrenciesManager
{
    QStringList _currencies;
    QString _defaultCurrency;

public:
    CurrenciesManager(QString defC, QStringList curs) : _currencies(curs), _defaultCurrency(defC) {};

    QStringList codes() const { return _currencies; };
    QString def() const { return _defaultCurrency; }
};

class Currency
{
    QString name, code, symbol;
    double rate;

public:
    Currency(QString n, QString c, QString s, double r) : name(n), code(c), symbol(s), rate(r) {}

    double convertToDefault(double amount) { return amount * rate; };
};

