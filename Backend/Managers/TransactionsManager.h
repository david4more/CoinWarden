#pragma once

#include "../Modules/Utils.h"
class QSqlDatabase;
class Transaction;

const QString transactionsTable = R"(
    CREATE TABLE transactions (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    amount REAL,
    currency TEXT,
    dateTime TEXT,
    category TEXT,
    account TEXT,
    note TEXT,
    FOREIGN KEY(amount) REFERENCES currencies(code),
    FOREIGN KEY(category) REFERENCES categories(id),
    FOREIGN KEY(account) REFERENCES accounts(id))
)";

class BACKEND_EXPORT TransactionsManager
{
    friend class Backend;

    QSqlDatabase& db;
    void normalizeMonthRange(QDate& from, QDate& to) const;

public:
    TransactionsManager(QSqlDatabase& db) : db(db) {}

    bool setupDefault();
    QVector<Transaction> get(const QDate& from, const QDate& to) const;
    QVector<QPair<QString, double>> transactionsPerCategory(const QDate& from, const QDate& to, CategoryType type = CategoryType::All) const;
    QVector<DailyTransactions> transactionsPerDay(const QDate& from, const QDate& to) const;
    bool add(const Transaction& t);
};
