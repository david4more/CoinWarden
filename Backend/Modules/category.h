#pragma once

class QSqlDatabase;
class Category;
const QString categoriesTable =
    "CREATE TABLE categories ("
    "name TEXT PRIMARY KEY, "
    "isExpense BOOLEAN NOT NULL, "
    "color TEXT)";

class CategoriesManager
{
    QSqlDatabase& db;
    static const QString defaultColor;

public:
    explicit CategoriesManager(QSqlDatabase& db) : db(db) {}
    QVector<Category> get() const;
    QStringList getNames() const;
    bool add(QString name, bool isExpense, QString color = defaultColor);
    bool setupDefault();
};
