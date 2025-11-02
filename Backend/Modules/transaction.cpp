#include "transaction.h"


void TransactionsManager::add(Transaction t)
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

QVector<Transaction> TransactionsManager::get(const QDate& from, const QDate& to)
{
    QVector<Transaction> transactions;

    QSqlQuery query(db);

    // SQLite date comparison works with 'YYYY-MM-DD' strings
    query.prepare("SELECT * "
                  "FROM transactions "
                  "WHERE date(dateTime) BETWEEN :from AND :to "
                  "ORDER BY date(dateTime) ASC");

    query.bindValue(":from", from.toString(Qt::ISODate));
    query.bindValue(":to", to.toString(Qt::ISODate));

    if (!query.exec()) qDebug() << "Failed to execute query";

    while (query.next()) {
        Transaction t;
        t.amount    = query.value("amount").toDouble();
        t.currency  = query.value("currency").toString();
        t.dateTime  = QDateTime::fromString(query.value("dateTime").toString(), Qt::ISODate);
        t.category  = query.value("category").toString();
        t.account   = query.value("account").toString();
        t.note      = query.value("note").toString();
        t.id        = query.value("id").toInt();

        transactions.push_back(std::move(t));
    }

    return transactions;
}
