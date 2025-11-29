#pragma once

#include "utils.h"
class QSqlDatabase;

const QString accountsTable = R"(
    CREATE TABLE accounts (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT UNIQUE NOT NULL)
)";

class BACKEND_EXPORT AccountsManager
{
    friend class Backend;
    bool setupDefault();

    QSqlDatabase& db;
    QStringList _names;

public:
    explicit AccountsManager(QSqlDatabase& db) : db(db), _names({ "User1", "User2", "User cat"}) {}
    QStringList names() const { return _names; }
};
