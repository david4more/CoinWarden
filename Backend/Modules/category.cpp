#include "category.h"
#include "c.h"
#include "pch.h"

const QString CategoriesManager::defaultColor = "#00ff00";

QVector<Category> CategoriesManager::get() const
{
    QSqlQuery query(db);

    query.prepare("SELECT name, color, isExpense FROM categories");

    if (!query.exec()) { qDebug() << "Failed to execute CategoriesManager::get query"; return {}; }

    QVector<Category> ret;

    while (query.next())
        ret.push_back(Category{query.value(0).toString(), query.value(1).toString(), query.value(2).toBool()});

    return ret;
}

QStringList CategoriesManager::getNames(CategoryType type) const
{
    QSqlQuery query(db);

    QString q = "SELECT name FROM categories";
    switch (type) {
    case CategoryType::Expense:
        q += " WHERE isExpense = 1"; break;
    case CategoryType::Income:
        q += " WHERE isExpense = 0"; break;
    }

    query.prepare(q);

    if (!query.exec()) { qDebug() << "Failed to execute CategoriesManager::getNames query"; return {}; }

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

    for (auto n : {"Food", "Entertainment", "Gifts", "Health", "Clothing", "Education", "Transport", "Household"})
        if (!add(n, true)) { db.rollback(); return false; }

    for (auto n : { "Salary", "Help", "Bonuses", "Gifts" })
        if (!add(n, false)) { db.rollback(); return false; }

    if (!db.commit()) { qDebug() << "Failed to commit transaction"; return false; }

    return true;
}

bool CategoriesManager::add(QString name, bool isExpense, QString color)
{
    QSqlQuery query(db);

    query.prepare("INSERT INTO categories (name, color, isExpense) VALUES (:name, :color, :isExpense)");

    query.bindValue(":name", name);
    query.bindValue(":color", color);
    query.bindValue(":isExpense", isExpense? 1 : 0);

    if (!query.exec()) { qDebug() << "Failed to execute CategoriesManager::add query"; return false; }

    return true;
}
















