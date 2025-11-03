#pragma once

class Category
{
public:
    Category(QString n, QString c, bool i) : name(n), color(c), isExpense(i) {}

    QString name, color;
    bool isExpense;
};
