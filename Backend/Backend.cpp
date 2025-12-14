#include "Backend.h"

#include "Managers/TransactionsManager.h"
#include "Managers/CurrenciesManager.h"
#include "Managers/CategoriesManager.h"
#include "Managers/AccountsManager.h"
#include "Modules/Transaction.h"

#include <QSqlQuery>
#include <QtDebug>
#include <QFile>

void Backend::initialize()
{
    if (initialized) return;
    const QString name = "main", path = "finance.db";

    if (QSqlDatabase::contains(name)) {
        qDebug() << "not first call";
        return;
    }

    bool isFirstLaunch = !QFile::exists(path);

    db = QSqlDatabase::addDatabase("QSQLITE", name);
    db.setDatabaseName(path);
    if (!db.open()) qDebug() << "Failed to open DB";

    _transactions = new TransactionsManager(db);
    _currencies = new CurrenciesManager(db);
    _accounts = new AccountsManager(db);
    _categories = new CategoriesManager(db);

    if (isFirstLaunch) {
        defaultSetupAvailable = true;
        QSqlQuery query(db);
        for (const auto& q : { categoriesTable, currenciesTable, accountsTable, transactionsTable })
            if (!query.exec(q)) qDebug() << "Failed to create table";

        emit firstLaunch();
    }
    _currencies->init();

    initialized = true;
}

bool Backend::setupDefault()
{
    if (!defaultSetupAvailable) return false;
    if (_transactions == nullptr || _categories == nullptr || _currencies == nullptr || _accounts == nullptr) return false;
    return (_categories->setupDefault() && _currencies->setupDefault() && _accounts->setupDefault() && _transactions->setupDefault());
}

Backend::~Backend()
{
    delete _transactions;
    delete _currencies;
    delete _accounts;
    delete _categories;
}

























