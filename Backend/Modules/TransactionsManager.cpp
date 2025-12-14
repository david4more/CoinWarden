#include "TransactionsManager.h"
#include "Transaction.h"
#include "pch.h"
#include <QDate>

bool TransactionsManager::add(const Transaction& t)
{
    QSqlQuery query(db);

    query.prepare(R"(
                  INSERT INTO transactions (amount, currency, dateTime, category, account, note)
                  VALUES (:amount, :currency, :dateTime, :category, :account, :note)
    )");

    query.bindValue(":amount", t.amount);
    query.bindValue(":currency", t.currency);
    query.bindValue(":dateTime", t.dateTime.toString(Qt::ISODate));
    query.bindValue(":category", t.category);
    query.bindValue(":account", t.account);
    query.bindValue(":note", t.note);

    if (!query.exec()) { qDebug() << "Failed to execute TransactionsManager::add query"; return false; }

    return true;
}

bool TransactionsManager::setupDefault()
{
    //QSqlQuery query(db);
    //query.prepare("SELECT 1 FROM transactions LIMIT 1");

    const QVector<QString> expenseCategories =
        {"Food", "Entertainment", "Gifts", "Health", "Clothing", "Education", "Transport", "Household"};
    const QVector<QString> incomeCategories  =
        {"Salary", "Help", "Bonuses" };

    QDate start = QDate::currentDate().addMonths(-1);
    srand(static_cast<unsigned int>(QTime::currentTime().msec()));

    for (int i = 0; i < 100; ++i) {
        bool isExpense = rand() % 2 == 0;
        QString category = isExpense
                               ? expenseCategories[rand() % expenseCategories.size()]
                               : incomeCategories[rand() % incomeCategories.size()];

        float amount;
        if (isExpense) {
            amount = -(5 + rand() % 95 + (rand() % 100) / 100.0); // -5 to -100
        } else {
            amount = 20 + rand() % 180 + (rand() % 100) / 100.0;  // 20 to 200
        }

        // Random date in last month
        QString account = "User1";

        if (!add({ amount, "UAH", QDateTime::currentDateTime().addDays(-(rand() % 30)), category, account })) return false;
    }

    return true;
}

QVector<DailyTransactions> TransactionsManager::transactionsPerDay(const QDate& from, const QDate& to) const
{
    QSqlQuery query(db);

    query.prepare(R"(
                  SELECT date(t.dateTime) AS day,
                  SUM(CASE WHEN t.amount < 0 THEN t.amount ELSE 0 END) AS expense,
                  SUM(CASE WHEN t.amount > 0 THEN t.amount ELSE 0 END) AS income
                  FROM transactions t
                  WHERE date(t.dateTime) BETWEEN :from AND :to
                  GROUP BY day
                  ORDER BY day ASC;
    )");

    query.bindValue(":from", from.toString(Qt::ISODate));
    query.bindValue(":to", to.toString(Qt::ISODate));

    if (!query.exec()) {
        qDebug() << "Failed to execute TransactionsManager::transactionsPerDay query";
        return {};
    }

    QVector<DailyTransactions> dailyData;
    while (query.next()) {
        DailyTransactions d;
        d.date = QDate::fromString(query.value(0).toString(), Qt::ISODate);
        d.expense = query.value(1).toDouble(); // will be negative if you store expenses as negative
        d.income = query.value(2).toDouble();
        dailyData.push_back(d);
    }

    return dailyData;
}

QVector<QPair<QString, double>> TransactionsManager::transactionsPerCategory(const QDate& from, const QDate& to, CategoryType type) const
{
    QSqlQuery query(db);

    QString sql = R"(
                  SELECT c.name, COALESCE(SUM(t.amount), 0) as amount
                  FROM categories c
                  LEFT JOIN transactions t
                  ON c.name = t.category
                  AND date(t.dateTime) BETWEEN :from AND :to
    )";

    switch (type)
    {
    case CategoryType::Expense:
        sql += " WHERE c.isExpense = 1 GROUP BY c.name ORDER BY amount ASC;"; break;
    case CategoryType::Income:
        sql += " WHERE c.isExpense = 0 GROUP BY c.name ORDER BY amount DESC;"; break;
    case CategoryType::All:
        sql += " GROUP BY c.name ORDER BY amount DESC;"; break;
    }

    query.prepare(sql);
    query.bindValue(":from", from.toString(Qt::ISODate));
    query.bindValue(":to", to.toString(Qt::ISODate));

    if (!query.exec()) { qDebug() << "Failed to execute TransactionsManager::expensePerCategory query"; return {}; }

    QVector<QPair<QString, double>> expenses;
    while (query.next()) expenses.push_back({ query.value(0).toString(), query.value(1).toDouble() });

    return expenses;
}

QVector<Transaction> TransactionsManager::get(const QDate& from, const QDate& to) const
{
    QSqlQuery query(db);

    query.prepare(R"(
                  SELECT amount, currency, dateTime, category, account, note, id
                  FROM transactions
                  WHERE date(dateTime) BETWEEN :from AND :to
                  ORDER BY date(dateTime) ASC
    )");

    query.bindValue(":from", from.toString(Qt::ISODate));
    query.bindValue(":to", to.toString(Qt::ISODate));

    if (!query.exec()) { qDebug() << "Failed to execute TransactionsManager::get query"; return {}; }

    QVector<Transaction> transactions;
    while (query.next()) {
        Transaction t;
        t.amount    = query.value(0).toDouble();
        t.currency  = query.value(1).toString();
        t.dateTime  = QDateTime::fromString(query.value(2).toString(), Qt::ISODate);
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















