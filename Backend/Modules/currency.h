#pragma once

#ifdef BACKEND_LIB
#  define BACKEND_EXPORT __declspec(dllexport)
#else
#  define BACKEND_EXPORT __declspec(dllimport)
#endif

class QSqlDatabase;

const QString currenciesTable = R"(
    CREATE TABLE currencies (
    code TEXT PRIMARY KEY,
    symbol TEXT UNIUE NOT NULL)
)";


class BACKEND_EXPORT Currency
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

