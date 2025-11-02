#pragma once

#include <QStringList>


const QString categoriesTable =
    "CREATE TABLE IF NOT EXISTS categories ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "name TEXT UNIQUE NOT NULL)";


class CategoriesManager
{
    QStringList _names;

public:
    CategoriesManager(QStringList ns) : _names(std::move(ns)) {}
    QStringList names() const  {return _names; }
};
