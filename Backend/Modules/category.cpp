#include "category.h"
#include "pch.h"

const QString CategoriesManager::defaultColor = "00ff00";

QStringList CategoriesManager::names() const
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

    return add({"Food", "Entertainment", "Gifts", "Health", "Clothing", "Education", "Transport", "Household"});
}

bool CategoriesManager::add(QStringList names, QString color)
{
    QSqlQuery query(db);

    if (!db.transaction()) { qDebug() << "Failed to initialize a transaction"; return false; }

    query.prepare("INSERT INTO categories (name, color) VALUES (:name, :color)");
    for (const auto& name : names) {
        query.bindValue(":name", name);
        query.bindValue(":color", color);
        if (!query.exec()) { qDebug() << "Failed to execute query"; db.rollback(); return false; }
    }

    if (!db.commit()) { qDebug() << "Failed to commit transaction"; return false; }
    return true;
}

bool CategoriesManager::add(QString name, QString color)
{
    return add({name}, color);
}
















