#pragma once

#include <QDateTime>

const QString transactionsTable = R"(
    CREATE TABLE transactions (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    amount REAL,
    dateTime TEXT,
    note TEXT,
    currency TEXT,
    category int,
    account int,
    FOREIGN KEY(currency) REFERENCES currencies(code),
    FOREIGN KEY(category) REFERENCES categories(id),
    FOREIGN KEY(account) REFERENCES accounts(id))
)";

class Transaction
{
public:
    // recurrence
    int id;
    double amount;
    QDateTime dateTime;
    QString note;
    QString currency;
    QString category;
    QString account;

    Transaction(double amount, QString currency, QDateTime dateTime, int category = -1, int budget = -1);
    Transaction() = default;

    explicit operator bool() const {
        return !(amount == 0.f || currency.isEmpty() || dateTime.isNull() || category.isEmpty() || account.isEmpty());
    }
};
