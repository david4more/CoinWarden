#pragma once

class QSqlDatabase;

const QString categoriesTable =
    "CREATE TABLE categories ("
    "name TEXT PRIMARY KEY, "
    "color TEXT)";

class CategoriesManager
{
    QSqlDatabase& db;
    static const QString defaultColor;

public:
    explicit CategoriesManager(QSqlDatabase& db) : db(db) {}
    QStringList names() const;
    bool add(QStringList names, QString color = defaultColor);
    bool add(QString name, QString color = defaultColor);
    bool setupDefault();
};
