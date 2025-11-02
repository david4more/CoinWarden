#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringList>
#include <QDateTime>


const QString transactionsTable =
    "CREATE TABLE IF NOT EXISTS transactions ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
    "amount REAL, "
    "currency TEXT, "
    "dateTime TEXT, "
    "category TEXT, "
    "account TEXT, "
    "note TEXT)";


class Transaction;
class TransactionsManager
{
    QSqlDatabase& db;

public:
    TransactionsManager(QSqlDatabase& db) : db(db) {}

    void add(Transaction t);
    QVector<Transaction> get(const QDate& from, const QDate& to);
};

class Transaction
{
public:
    // recurrence
    float amount;
    QString currency;
    QDateTime dateTime;
    QString category;
    QString account;
    QString note;
    int id;

    explicit operator bool() const {
        return !(amount == 0.f || currency.isEmpty() || dateTime.isNull() || category.isEmpty() || account.isEmpty());
    }
};
