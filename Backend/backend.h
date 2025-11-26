#pragma once

#ifdef BACKEND_LIB
#  define BACKEND_EXPORT __declspec(dllexport)
#else
#  define BACKEND_EXPORT __declspec(dllimport)
#endif

// #include <QObject>
#include <QSqlDatabase>
class TransactionsManager;
class CurrenciesManager;
class CategoriesManager;
class AccountsManager;

class BACKEND_EXPORT Backend {
/*
        Q_OBJECT

signals:
    void firstLaunch();
*/

public:
    // Backend(QObject* parent) : QObject(parent) {}   // init() call after connecting signals required
    Backend();
    ~Backend();
    void init();

    TransactionsManager* transactions() { return _transactions; }
    CurrenciesManager* currencies() { return _currencies; }
    AccountsManager* accounts() { return _accounts; }
    CategoriesManager* categories() { return _categories; }

private:
    QSqlDatabase db;
    TransactionsManager* _transactions;
    CurrenciesManager* _currencies;
    AccountsManager* _accounts;
    CategoriesManager* _categories;
};
