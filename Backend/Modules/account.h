#pragma once

class QSqlDatabase;

const QString accountsTable =
    "CREATE TABLE accounts ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "name TEXT UNIQUE NOT NULL)";

class AccountsManager
{
    QSqlDatabase& db;
    QStringList _names;

public:
    AccountsManager(QSqlDatabase& db) : db(db) { _names = { "User1", "User2", "User cat"}; }
    QStringList names() const { return _names; }
};
