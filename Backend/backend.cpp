#include "backend.h"

#include "Modules/transaction.h"
#include "Modules/currency.h"
#include "Modules/category.h"
#include "Modules/account.h"
#include <QSqlQuery>
#include <QtDebug>
#include <QFile>

void Backend::init()
{
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
        QSqlQuery query(db);
        for (const auto& q : { categoriesTable, currenciesTable, accountsTable, transactionsTable })
            if (!query.exec(q)) qDebug() << "Failed to create table";

        emit firstLaunch();
    }
}

Backend::~Backend()
{
    delete _transactions;
    delete _currencies;
    delete _accounts;
    delete _categories;
}

























