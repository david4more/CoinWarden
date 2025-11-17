#pragma once
#include <QDate>

enum class CategoryType { All, Expense, Income };

struct DailyTransactions { QDate date; double expense; double income; };
