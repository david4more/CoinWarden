#pragma once

#include "Modules/transaction.h"
#include "Modules/currency.h"
#include "Modules/category.h"
#include "Modules/account.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtDebug>
#include <QNetworkAccessManager>

class Backend
{
public:
    Backend();
    TransactionsManager& transactions() { return *_transactions.data(); }
    CurrenciesManager& currencies() { return *_currencies.data(); }
    CategoriesManager& categories() { return *_categories.data(); }
    AccountsManager& accounts() { return *_accounts.data(); }

private:
    QSqlDatabase db;
    QNetworkAccessManager network;

    QScopedPointer<TransactionsManager> _transactions;
    QScopedPointer<CurrenciesManager> _currencies;
    QScopedPointer<CategoriesManager> _categories;
    QScopedPointer<AccountsManager> _accounts;
};
















