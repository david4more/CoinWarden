#pragma once

class QSqlDatabase;
class Category;
const QString categoriesTable =
    "CREATE TABLE categories ("
    "name TEXT NOT NULL, "
    "isExpense BOOLEAN NOT NULL, "
    "color TEXT, "
    "PRIMARY KEY (name, isExpense))";

class CategoriesManager
{
    QSqlDatabase& db;
    static const QString defaultColor;

public:
    explicit CategoriesManager(QSqlDatabase& db) : db(db) {}
    QVector<Category> get() const;
    QStringList getExpenseNames() const;
    QStringList getIncomeNames() const;
    QStringList getNames() const;
    bool add(QString name, bool isExpense, QString color = defaultColor);
    bool setupDefault();
};
