#pragma once

#include <QStringList>


const QString accountsTable =
    "CREATE TABLE IF NOT EXISTS accounts ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "name TEXT UNIQUE NOT NULL)";


class AccountsManager
{
    QStringList _names;

public:
    AccountsManager(QStringList names) : _names(std::move(names)) {}
    QStringList names() const { return _names; }
};
