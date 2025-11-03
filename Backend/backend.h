#pragma once

#include <QObject>
#include <QSqlDatabase>
class TransactionsManager;
class CurrenciesManager;
class CategoriesManager;
class AccountsManager;
class QSqlDatabase;

class Backend : public QObject {
    Q_OBJECT

signals:
    void firstLaunch();

public:
    Backend(QObject* parent) : QObject(parent) {}   // init() call after connecting signals required
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
