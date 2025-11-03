#include "currency.h"

CurrenciesManager::CurrenciesManager(QSqlDatabase& db) : db(db)
{
    _defaultCurrency = "EUR";
    _currencies = { "EUR", "GBP", "CHF", "PLN", "UAH", "USD", "CAD", "JPY", "CNY" };
}
