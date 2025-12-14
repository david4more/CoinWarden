#pragma once

#include <QDateTime>

const QString transactionsTable = R"(
    CREATE TABLE transactions (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    amount REAL,
    dateTime TEXT,
    note TEXT,
    currency TEXT,
    category INTEGER,
    account INTEGER,
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
    int category = -1;
    QString account;

    Transaction() = default;
    Transaction(double amount, QString currency, QDateTime dateTime, int category, QString budget) :
        amount(amount), currency(currency), dateTime(dateTime), category(category), account(budget) {}

    explicit operator bool() const {
        return !(amount == 0.f || currency.isEmpty() || dateTime.isNull() || category < 0 || account.isEmpty());
    }
};
