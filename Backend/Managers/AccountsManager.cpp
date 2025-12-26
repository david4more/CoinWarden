#include "AccountsManager.h"

bool AccountsManager::setupDefault()
{
    QSqlQuery query(db);
    query.prepare("SELECT 1 FROM accounts LIMIT 1");
    if (!query.exec()) { qDebug() << "Failed to execute query"; return false; }
    if (query.next()) { qDebug() << "Setup of non-empty table"; return false; }

    if (!db.transaction()) { qDebug() << "Failed to initialize a transaction"; return false; }

    for (auto n : { "Salary", "Savings", "Family budget", "Alex's help", "Debt" })
        if (!add(n)) { db.rollback(); return false; }

    if (!db.commit()) { qDebug() << "Failed to commit transaction"; return false; }

    return true;
}

bool AccountsManager::init()
{
    QSqlQuery query(db);

    query.prepare("SELECT name FROM accounts");

    if (!query.exec()) { qDebug() << "Failed to execute AccountsManager::init query"; return {}; }

    while (query.next())
        names.push_back(query.value(0).toString());

    return true;
}

bool AccountsManager::add(QString name)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO accounts (name) VALUES (:name)");
    query.bindValue(":name", name);

    if (!query.exec()) { qDebug() << "Failed to execute AccountsManager::add query"; return false; }

    return true;
}
