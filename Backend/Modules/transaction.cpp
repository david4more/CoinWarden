#include "transaction.h"
#include "t.h"
#include "pch.h"
#include <QDate>

void TransactionsManager::add(const Transaction& t)
{
    QSqlQuery query(db);

    query.prepare("INSERT INTO transactions (amount, currency, dateTime, category, account, note) "
                  "VALUES (:amount, :currency, :dateTime, :category, :account, :note)");

    query.bindValue(":amount", t.amount);
    query.bindValue(":currency", t.currency);
    query.bindValue(":dateTime", t.dateTime.toString(Qt::ISODate));
    query.bindValue(":category", t.category);
    query.bindValue(":account", t.account);
    query.bindValue(":note", t.note);

    if (!query.exec()) qDebug() << "Failed to execute query";
}

QVector<QPair<QDate, double>> TransactionsManager::expensePerDay(const QDate& from, const QDate& to) const
{
    QSqlQuery query(db);

    query.prepare("SELECT date(t.dateTime) AS day, SUM(t.amount) AS total "
                  "FROM transactions t "
                  "WHERE date(t.dateTime) BETWEEN :from AND :to "
                  "GROUP BY day "
                  "ORDER BY day ASC;");

    query.bindValue(":from", from.toString(Qt::ISODate));
    query.bindValue(":to", to.toString(Qt::ISODate));

    if (!query.exec()) { qDebug() << "Failed to execute query"; return {}; }

    QVector<QPair<QDate, double>> expenses;
    while (query.next()) expenses.push_back({ QDate::fromString(query.value(0).toString(), Qt::ISODate), query.value(1).toDouble() });

    return expenses;
}

QVector<QPair<QString, double>> TransactionsManager::expensePerCategory(const QDate& from, const QDate& to) const
{
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

    if (!query.exec()) { qDebug() << "Failed to execute query"; return {}; }

    QVector<QPair<QString, double>> expenses;
    while (query.next()) expenses.push_back({ query.value(0).toString(), query.value(1).toDouble() });

    return expenses;
}

QVector<Transaction> TransactionsManager::get(const QDate& from, const QDate& to) const
{
    QSqlQuery query(db);

    query.prepare("SELECT amount, currency, dateTime, category, account, note, id "
                  "FROM transactions "
                  "WHERE date(dateTime) BETWEEN :from AND :to "
                  "ORDER BY date(dateTime) ASC");

    query.bindValue(":from", from.toString(Qt::ISODate));
    query.bindValue(":to", to.toString(Qt::ISODate));

    if (!query.exec()) { qDebug() << "Failed to execute query"; return {}; }

    QVector<Transaction> transactions;
    while (query.next()) {
        Transaction t;
        t.amount    = query.value(0).toDouble();
        t.currency  = query.value(1).toString();
        t.dateTime  = QDateTime::fromString(query.value(2).toString());
        t.category  = query.value(3).toString();
        t.account   = query.value(4).toString();
        t.note      = query.value(5).toString();
        t.id        = query.value(6).toInt();

        transactions.push_back(std::move(t));
    }

    return transactions;
}

void TransactionsManager::normalizeMonthRange(QDate& from, QDate& to) const
{
    from = QDate(from.year(), from.month(), 1);
    to = QDate(to.year(), to.month(), QDate(to.year(), to.month(), 1).daysInMonth());
}















