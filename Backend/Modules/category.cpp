#include "category.h"
#include "c.h"
#include "pch.h"

const QString CategoriesManager::defaultColor = "00ff00";

QVector<Category> CategoriesManager::get() const
{
    QSqlQuery query(db);

    query.prepare("SELECT name, color, isExpense FROM categories");

    if (!query.exec()) { qDebug() << "Failed to execute query"; return {}; }

    QVector<Category> ret;

    while (query.next())
        ret.push_back(Category{query.value(0).toString(), query.value(1).toString(), query.value(2).toBool()});

    return ret;
}

QStringList CategoriesManager::getNames() const
{
    QSqlQuery query(db);

    query.prepare("SELECT name FROM categories");

    if (!query.exec()) { qDebug() << "Failed to execute query"; return {}; }

    QStringList ret;
    while (query.next())
        ret << query.value(0).toString();

    return ret;
}

bool CategoriesManager::setupDefault()
{
    QSqlQuery query(db);
    query.prepare("SELECT 1 FROM categories LIMIT 1");
    if (!query.exec()) { qDebug() << "Failed to execute query"; return false; }
    if (query.next()) { qDebug() << "Setup of non-empty table"; return false; }

    if (!db.transaction()) { qDebug() << "Failed to initialize a transaction"; return false; }

    qDebug() << "1";
    for (auto n : {"Food", "Entertainment", "Gifts", "Health", "Clothing", "Education", "Transport", "Household"})
        if (!add(n, true)) { db.rollback(); return false; }

    //for (auto n : { "Salary", "Help", "Bonuses", "Gifts" })
        //if (!add(n, false)) { db.rollback(); return false; }

    if (!db.commit()) { qDebug() << "Failed to commit transaction"; return false; }

    return true;
}

bool CategoriesManager::add(QString name, bool isExpense, QString color)
{
    QSqlQuery query(db);

    qDebug() << "2";
    query.prepare("INSERT INTO categories (name, color, isExpense) VALUES (:name, :color, :isExpense)");

    query.bindValue(":name", name);
    query.bindValue(":color", color);
    query.bindValue(":isExpense", isExpense);

    if (!query.exec()) { qDebug() << "Failed to execute query"; return false; }

    return true;
}
















