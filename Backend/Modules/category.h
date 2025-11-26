#pragma once

#ifdef BACKEND_LIB
#  define BACKEND_EXPORT __declspec(dllexport)
#else
#  define BACKEND_EXPORT __declspec(dllimport)
#endif


#include "utils.h"
class QSqlDatabase;
class Category;
const QString categoriesTable = R"(
    CREATE TABLE categories (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    isExpense BOOLEAN NOT NULL,
    color TEXT)
)";

class BACKEND_EXPORT CategoriesManager
{
    QSqlDatabase& db;
    static const QString defaultColor;

public:
    explicit CategoriesManager(QSqlDatabase& db) : db(db) {}
    QVector<Category> get() const;
    QStringList getNames(CategoryType type = CategoryType::All) const;
    bool add(QString name, bool isExpense, QString color = defaultColor);
    bool setupDefault();
};
