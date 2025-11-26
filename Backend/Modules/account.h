#pragma once

#ifdef BACKEND_LIB
#  define BACKEND_EXPORT __declspec(dllexport)
#else
#  define BACKEND_EXPORT __declspec(dllimport)
#endif

class QSqlDatabase;

const QString accountsTable = R"(
    CREATE TABLE accounts (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT UNIQUE NOT NULL)
)";

class BACKEND_EXPORT AccountsManager
{
    QSqlDatabase& db;
    QStringList _names;

public:
    AccountsManager(QSqlDatabase& db) : db(db) { _names = { "User1", "User2", "User cat"}; }
    QStringList names() const { return _names; }
};
