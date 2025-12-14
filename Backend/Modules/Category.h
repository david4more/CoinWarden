#pragma once

const QString categoriesTable = R"(
    CREATE TABLE categories (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    isExpense BOOLEAN NOT NULL,
    monthlyLimit INTEGER,
    color TEXT)
)";

class Category
{
public:
    explicit Category(int id, QString n, bool i, QString c) : name(n), color(c), isExpense(i), id(id) {}

    int id, monthlyLimit;
    QString name, color;
    bool isExpense;
};
