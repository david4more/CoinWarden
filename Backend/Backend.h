#pragma once

#include "Modules/Utils.h"
#include <QObject>
#include <QSqlDatabase>
class TransactionsManager;
class CurrenciesManager;
class CategoriesManager;
class AccountsManager;

class BACKEND_EXPORT Backend : public QObject
{
        Q_OBJECT

signals:
    void firstLaunch();

public:
    Backend(QObject* parent = nullptr) : QObject(parent) {}   // initialize() call after connecting signals required
    ~Backend();
    void initialize();

    TransactionsManager* transactions() { return _transactions; }
    CurrenciesManager* currencies() { return _currencies; }
    AccountsManager* accounts() { return _accounts; }
    CategoriesManager* categories() { return _categories; }

    bool setupDefault();

private:
    bool initialized = false;
    bool defaultSetupAvailable = false;
    QSqlDatabase db;
    TransactionsManager* _transactions = nullptr;
    CurrenciesManager* _currencies = nullptr;
    AccountsManager* _accounts = nullptr;
    CategoriesManager* _categories = nullptr;
};
