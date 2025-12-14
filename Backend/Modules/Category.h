#pragma once

const QString categoriesTable = R"(
    CREATE TABLE categories (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    isExpense BOOLEAN NOT NULL,
    color TEXT)
)";

class Category
{
public:
    explicit Category(int id, QString n, bool i, QString c) : name(n), color(c), isExpense(i), id(id) {}

    int id;
    QString name, color;
    bool isExpense;
};
