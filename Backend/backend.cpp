#include "backend.h"

Backend::Backend()
{
    if (!QSqlDatabase::contains("main")) {
        db = QSqlDatabase::addDatabase("QSQLITE", "main");
        db.setDatabaseName("finance.db");
    } else
        db = QSqlDatabase::database("main");

    if (!db.open()) {
        qDebug() << "Failed to open DB";
        return;
    }

    auto execQueries = [&](const QStringList &queries) {
        QSqlQuery query(db);
        for (const auto& q : queries){
            if (!query.exec(q)){
                qDebug() << "Failed to create table(s)";
                return false;
            }
        }
        return true;
    };

    if (!execQueries({transactionsTable, currenciesTable, accountsTable, categoriesTable}))
        qDebug() << "Failed to setup DB";



    QStringList currencyCodes = {
        // Europe
        "EUR", // Euro
        "GBP", // British Pound
        "CHF", // Swiss Franc
        "PLN", // Polish Zloty
        "UAH", // Ukrainian Hryvnia
        // America
        "USD", // US Dollar
        "CAD", // Canadian Dollar
        // Asia
        "JPY", // Japanese Yen
        "CNY", // Chinese Yuan
    };

    _accounts.reset(new AccountsManager({"User1", "User2", "Muhehehehe"}));
    _categories.reset(new CategoriesManager({ "Food", "Entertainment", "Hehehoho", "Health", "Gifts" }));
    _currencies.reset(new CurrenciesManager("EUR", currencyCodes));
    _transactions.reset(new TransactionsManager(db));
}



























