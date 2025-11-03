#pragma once

class QSqlDatabase;
class Transaction;

const QString transactionsTable =
    "CREATE TABLE transactions ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
    "amount REAL, "
    "currency TEXT, "
    "dateTime TEXT, "
    "category TEXT, "
    "account TEXT, "
    "note TEXT, "
    "FOREIGN KEY(amount) REFERENCES currencies(code), "
    "FOREIGN KEY(category) REFERENCES categories(name), "
    "FOREIGN KEY(account) REFERENCES accounts(name))";

class TransactionsManager
{
    QSqlDatabase& db;

public:
    TransactionsManager(QSqlDatabase& db) : db(db) {}

    void add(const Transaction& t);
    QVector<Transaction> get(const QDate& from, const QDate& to) const;
    QVector<QPair<QString, double>> expensePerCategory(const QDate& from, const QDate& to) const;
};
