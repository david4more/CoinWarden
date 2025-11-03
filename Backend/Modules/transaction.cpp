#include "transaction.h"
#include "t.h"
#include "pch.h"
#include <QDate>

void TransactionsManager::add(const Transaction& t)
{
    QSqlQuery query(db);
    query.prepare(
        "INSERT INTO transactions (amount, currency, dateTime, category, account, note) "
        "VALUES (:amount, :currency, :dateTime, :category, :account, :note)"
        );

    query.bindValue(":amount", t.amount);
    query.bindValue(":currency", t.currency);
    query.bindValue(":dateTime", t.dateTime);
    query.bindValue(":category", t.category);
    query.bindValue(":account", t.account);
    query.bindValue(":note", t.note);

    if (!query.exec()) qDebug() << "Failed to execute query";
}

QVector<QPair<QString, double>> TransactionsManager::expensePerCategory(const QDate& from, const QDate& to) const
{
    QVector<QPair<QString, double>> expenses;

    QSqlQuery query(db);

    query.prepare("SELECT c.name, COALESCE(SUM(t.amount), 0) as amount "
                  "FROM categories c "
                  "LEFT JOIN transactions t "
                  "ON c.name = t.category "
                  "AND date(t.dateTime) BETWEEN :from AND :to "
                  "GROUP BY c.name "
                  "ORDER BY amount DESC;");

    query.bindValue(":from", from.toString(Qt::ISODate));
    query.bindValue(":to", to.toString(Qt::ISODate));

    if (!query.exec()) qDebug() << "Failed to execute query";

    while (query.next()) {
        QString category = query.value(0).toString();
        double sum = query.value(1).toDouble();
        expenses.push_back({category, sum});
    }

    return expenses;
}

QVector<Transaction> TransactionsManager::get(const QDate& from, const QDate& to) const
{
    QVector<Transaction> transactions;

    QSqlQuery query(db);

    query.prepare("SELECT amount, currency, dateTime, category, account, note, id "
                  "FROM transactions "
                  "WHERE date(dateTime) BETWEEN :from AND :to "
                  "ORDER BY date(dateTime) ASC");

    query.bindValue(":from", from.toString(Qt::ISODate));
    query.bindValue(":to", to.toString(Qt::ISODate));

    if (!query.exec()) qDebug() << "Failed to execute query";

    while (query.next()) {
        Transaction t;
        t.amount    = query.value(0).toDouble();
        t.currency  = query.value(1).toString();
        t.dateTime  = query.value(2).toString();
        t.category  = query.value(3).toString();
        t.account   = query.value(4).toString();
        t.note      = query.value(5).toString();
        t.id        = query.value(6).toInt();

        transactions.push_back(std::move(t));
    }

    return transactions;
}
