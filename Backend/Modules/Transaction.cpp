#include "Transaction.h"


Transaction::Transaction(double amount, QString currency, QDateTime dateTime, int category, int budget) :
    amount(amount), currency(currency), dateTime(dateTime)
{

}